#include "core/RegularExpr.hpp"

#include "core/Symbol.hpp"

#include "regex/nodes/AlternExpr.hpp"
#include "regex/nodes/ConcatExpr.hpp"
#include "regex/nodes/ExprNode.hpp"
#include "regex/nodes/NodeVisitor.hpp"
#include "regex/nodes/OptionalExpr.hpp"
#include "regex/nodes/PlusClosure.hpp"
#include "regex/nodes/StarClosure.hpp"
#include "regex/nodes/SymbolAtom.hpp"
#include "regex/Parser.hpp"
#include "regex/make.hpp"

#include <algorithm>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

namespace parsec {
    using namespace regex;


    class RegularExpr::ComputeCache {
    public:

        ComputeCache(NodePtr rootNode)
            : regexWithEndMark_(std::move(rootNode), atom('$')) {}


        const Symbol& posValue(int pos) {
            class Impl : private NodeVisitor {
            public:
                AtomList operator()(const ExprNode& n) {
                    n.accept(*this);
                    return std::move(atoms_);
                }

            private:
                void visit(const SymbolAtom& n) override {
                    if(n.value()) {
                        atoms_.push_back(&n);
                    }
                }

                void visit(const PlusClosure& n) override {
                    n.inner()->accept(*this);
                }

                void visit(const StarClosure& n) override {
                    n.inner()->accept(*this);
                }

                void visit(const OptionalExpr& n) override {
                    n.inner()->accept(*this);
                }

                void visit(const AlternExpr& n) override {
                    n.left()->accept(*this);
                    n.right()->accept(*this);
                }

                void visit(const ConcatExpr& n) override {
                    n.left()->accept(*this);
                    n.right()->accept(*this);
                }

                AtomList atoms_;
            } impl;

            if(atoms_.empty()) {
                atoms_ = impl(*regexWithEndMark_.left());
            }

            if(pos >= 0 && pos < atoms_.size()) {
                return atoms_[pos]->value();
            }
            return emptyPosValue;
        }


        const PosList& followPos(int pos) {
            class Impl : private NodeVisitor {
            public:

                std::vector<PosList> operator()(const ExprNode& n) {
                    n.accept(*this);
                    return std::move(followPos_);
                }

            private:
                void visit(const SymbolAtom& /* n*/) override {
                    atomCount_++;
                }

                void visit(const PlusClosure& n) override {
                    addFirstPosToLastPos(*n.inner(), *n.inner(), atomCount_);
                    n.inner()->accept(*this);
                }

                void visit(const StarClosure& n) override {
                    addFirstPosToLastPos(*n.inner(), *n.inner(), atomCount_);
                    n.inner()->accept(*this);
                }

                void visit(const OptionalExpr& n) override {
                    n.inner()->accept(*this);
                }

                void visit(const AlternExpr& n) override {
                    n.left()->accept(*this);
                    n.right()->accept(*this);
                }

                void visit(const ConcatExpr& n) override {
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

                std::vector<PosList> followPos_;
                int atomCount_ = 0;
            };

            if(followPos_.empty()) {
                followPos_ = Impl()(regexWithEndMark_);

                // make sure that the followpos sets are all sorted and contain only unique elements
                for(auto& follow : followPos_) {
                    const auto unique = (std::ranges::sort(follow), std::ranges::unique(follow));
                    follow.erase(unique.begin(), unique.end());
                }
            }

            if(pos >= 0 && pos < followPos_.size()) {
                return followPos_[pos];
            }
            return emptyPosList;
        }


        const PosList& firstPos() {
            if(firstPos_.empty()) {
                firstPos_ = computeFirstOrLastPos(regexWithEndMark_, 0, true);
            }
            return firstPos_;
        }


    private:
        using AtomList = std::vector<const SymbolAtom*>;

        static PosList computeFirstOrLastPos(const ExprNode& n, int nextAtomIndex, bool firstOrLast) {
            class Impl : private NodeVisitor {
            public:

                Impl(int nextAtomIndex, bool firstOrLast) noexcept
                    : nextAtomIndex_(nextAtomIndex), firstOrLast_(firstOrLast) {}

                PosList operator()(const ExprNode& n) {
                    n.accept(*this);
                    return std::move(indices_);
                }

            private:
                void visit(const SymbolAtom& n) override {
                    if(n.value()) {
                        // only non-empty atoms must have positional indices assigned
                        indices_.push_back(nextAtomIndex_);
                    }
                }

                void visit(const PlusClosure& n) override {
                    n.inner()->accept(*this);
                }

                void visit(const StarClosure& n) override {
                    n.inner()->accept(*this);
                }

                void visit(const OptionalExpr& n) override {
                    n.inner()->accept(*this);
                }

                void visit(const AlternExpr& n) override {
                    n.left()->accept(*this);

                    const auto leftSize = n.left()->atomCount();
                    nextAtomIndex_ += leftSize;
                    n.right()->accept(*this);
                    nextAtomIndex_ -= leftSize;
                }

                void visit(const ConcatExpr& n) override {
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

                PosList indices_;
                int nextAtomIndex_ = {};
                bool firstOrLast_ = {};
            };

            return Impl(nextAtomIndex, firstOrLast)(n);
        }

        ConcatExpr regexWithEndMark_;

        PosList firstPos_;
        std::vector<PosList> followPos_;
        AtomList atoms_;
    };


    RegularExpr::RegularExpr(std::string_view regex)
        : RegularExpr(Parser::parseFrom(regex)) {}


    const RegularExpr::PosList& RegularExpr::firstPos() const {
        if(auto* const cache = computeCache()) {
            return cache->firstPos();
        }
        return emptyPosList;
    }


    const RegularExpr::PosList& RegularExpr::followPos(int pos) const {
        if(auto* const cache = computeCache()) {
            return cache->followPos(pos);
        }
        return emptyPosList;
    }


    const Symbol& RegularExpr::posValue(int pos) const {
        if(auto* const cache = computeCache()) {
            return cache->posValue(pos);
        }
        return emptyPosValue;
    }


    RegularExpr::ComputeCache* RegularExpr::computeCache() const {
        if(rootNode_ && !computeCache_) {
            computeCache_ = std::make_shared<ComputeCache>(rootNode_);
        }
        return computeCache_.get();
    }
}
