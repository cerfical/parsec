#include "regex/ast/AlternExprNode.hpp"
#include "regex/ast/AtomExprNode.hpp"
#include "regex/ast/ConcatExprNode.hpp"
#include "regex/ast/NodeVisitor.hpp"
#include "regex/ast/OptionalExprNode.hpp"
#include "regex/ast/PlusClosureNode.hpp"
#include "regex/ast/StarClosureNode.hpp"

#include "util/string_util.hpp"

namespace parsec::regex {
    std::ostream& operator<<(std::ostream& out, const ExprNode& n) {
        class Impl : private NodeVisitor {
        public:

            Impl(std::ostream& out)
                : out_(out) {}

            void operator()(const ExprNode& n) {
                n.accept(*this);
            }

        private:
            void visit(const AtomExprNode& n) override {
                if(n.value()) {
                    out_ << '\'' << string_util::escape(n.value().text()) << '\'';
                } else {
                    out_ << "()";
                }
            }

            void visit(const OptionalExprNode& n) override {
                n.inner()->accept(*this);
                out_ << '?';
            }

            void visit(const PlusClosureNode& n) override {
                n.inner()->accept(*this);
                out_ << '+';
            }

            void visit(const StarClosureNode& n) override {
                n.inner()->accept(*this);
                out_ << '*';
            }

            void visit(const AlternExprNode& n) override {
                out_ << '(';
                n.left()->accept(*this);
                out_ << " | ";
                n.right()->accept(*this);
                out_ << ')';
            }

            void visit(const ConcatExprNode& n) override {
                out_ << '(';
                n.left()->accept(*this);
                out_ << ' ';
                n.right()->accept(*this);
                out_ << ')';
            }


            std::ostream& out_;
        } impl(out);
        impl(n);

        return out;
    }


    void AtomExprNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void StarClosureNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void PlusClosureNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void OptionalExprNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void ConcatExprNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void AlternExprNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }
}
