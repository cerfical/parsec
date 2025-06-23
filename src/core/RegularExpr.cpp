#include "core/RegularExpr.hpp"
#include "core/Symbol.hpp"

#include "regex/Parser.hpp"
#include "regex/ast/AlternExprNode.hpp"
#include "regex/ast/AtomExprNode.hpp"
#include "regex/ast/ConcatExprNode.hpp"
#include "regex/ast/ExprNode.hpp"
#include "regex/ast/NodeVisitor.hpp"
#include "regex/ast/OptionalExprNode.hpp"
#include "regex/ast/PlusClosureNode.hpp"
#include "regex/ast/StarClosureNode.hpp"
#include "regex/make.hpp"

#include <algorithm>
#include <string_view>
#include <utility>
#include <vector>

using namespace parsec::regex;
using namespace parsec;

namespace {
    std::vector<int> computeFirstOrLastPos(const ExprNode& n, int nextAtomIndex, bool firstOrLast) {
        class Impl : private NodeVisitor {
        public:

            Impl(int nextAtomIndex, bool firstOrLast)
                : nextAtomIndex_(nextAtomIndex), firstOrLast_(firstOrLast) {}

            std::vector<int> run(const ExprNode& n) {
                n.accept(*this);
                return std::move(indices_);
            }

        private:
            void visit(const AtomExprNode& n) override {
                if(n.value()) {
                    // only non-empty atoms must have positional indices assigned
                    indices_.push_back(nextAtomIndex_);
                }
            }

            void visit(const PlusClosureNode& n) override {
                n.inner()->accept(*this);
            }

            void visit(const StarClosureNode& n) override {
                n.inner()->accept(*this);
            }

            void visit(const OptionalExprNode& n) override {
                n.inner()->accept(*this);
            }

            void visit(const AlternExprNode& n) override {
                n.left()->accept(*this);

                const auto leftSize = n.left()->atomCount();
                nextAtomIndex_ += leftSize;
                n.right()->accept(*this);
                nextAtomIndex_ -= leftSize;
            }

            void visit(const ConcatExprNode& n) override {
                if(firstOrLast_ || n.right()->isNullable()) {
                    n.left()->accept(*this);
                }

                if(!firstOrLast_ || n.left()->isNullable()) {
                    const auto leftSize = n.left()->atomCount();
                    nextAtomIndex_ += leftSize;
                    n.right()->accept(*this);
                    nextAtomIndex_ -= leftSize;
                }
            }

            std::vector<int> indices_;
            int nextAtomIndex_ = {};
            bool firstOrLast_ = {};
        };

        return Impl(nextAtomIndex, firstOrLast).run(n);
    }


    std::vector<Symbol> collectAtoms(const ExprNode& n) {
        class Impl : private NodeVisitor {
        public:

            std::vector<Symbol> run(const ExprNode& n) {
                n.accept(*this);
                return std::move(symbols_);
            }

        private:
            void visit(const AtomExprNode& n) override {
                if(n.value()) {
                    symbols_.push_back(n.value());
                }
            }

            void visit(const PlusClosureNode& n) override {
                n.inner()->accept(*this);
            }

            void visit(const StarClosureNode& n) override {
                n.inner()->accept(*this);
            }

            void visit(const OptionalExprNode& n) override {
                n.inner()->accept(*this);
            }

            void visit(const AlternExprNode& n) override {
                n.left()->accept(*this);
                n.right()->accept(*this);
            }

            void visit(const ConcatExprNode& n) override {
                n.left()->accept(*this);
                n.right()->accept(*this);
            }

            std::vector<Symbol> symbols_;
        };

        return Impl().run(n);
    }


    std::vector<std::vector<int>> computeFollowPos(const ExprNode& n) {
        class Impl : private NodeVisitor {
        public:

            std::vector<std::vector<int>> run(const ExprNode& n) {
                n.accept(*this);

                // make sure that the followpos sets are all sorted and contain only unique elements
                for(auto& follow : followPos_) {
                    const auto unique = (std::ranges::sort(follow), std::ranges::unique(follow));
                    follow.erase(unique.begin(), unique.end());
                }
                return std::move(followPos_);
            }

        private:
            void visit(const AtomExprNode& /* n*/) override {
                atomCount_++;
            }

            void visit(const PlusClosureNode& n) override {
                addFirstPosToLastPos(*n.inner(), *n.inner(), atomCount_);
                n.inner()->accept(*this);
            }

            void visit(const StarClosureNode& n) override {
                addFirstPosToLastPos(*n.inner(), *n.inner(), atomCount_);
                n.inner()->accept(*this);
            }

            void visit(const OptionalExprNode& n) override {
                n.inner()->accept(*this);
            }

            void visit(const AlternExprNode& n) override {
                n.left()->accept(*this);
                n.right()->accept(*this);
            }

            void visit(const ConcatExprNode& n) override {
                const auto preLeftAtomCount = atomCount_;
                n.left()->accept(*this);

                addFirstPosToLastPos(*n.left(), *n.right(), preLeftAtomCount);
                n.right()->accept(*this);
            }

            void addFirstPosToLastPos(const ExprNode& last, const ExprNode& first, int lastPosBaseAtomIndex) {
                const auto lastPos = computeFirstOrLastPos(last, lastPosBaseAtomIndex, false);
                if(!lastPos.empty()) {
                    const auto firstPos = computeFirstOrLastPos(first, atomCount_, true);
                    for(const auto& pos : lastPos) {
                        if(pos >= followPos_.size()) {
                            followPos_.resize(pos + 1);
                        }
                        followPos_[pos].insert(followPos_[pos].end(), firstPos.begin(), firstPos.end());
                    }
                }
            }

            std::vector<std::vector<int>> followPos_;
            int atomCount_ = 0;
        };

        return Impl().run(n);
    }
}

namespace parsec {
    RegularExpr::RegularExpr(NodePtr rootNode) {
        auto state = std::make_shared<State>();

        const auto regex = ConcatExprNode(std::move(rootNode), atom('$'));
        state->symbols = collectAtoms(*regex.left());
        state->firstPos = computeFirstOrLastPos(regex, 0, true);
        state->followPos = computeFollowPos(regex);

        state_ = std::move(state);
    }

    RegularExpr::RegularExpr(std::string_view regex)
        : RegularExpr(Parser::parseFrom(regex)) {}
}
