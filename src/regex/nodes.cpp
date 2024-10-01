#include "regex/nodes/AlternExpr.hpp"
#include "regex/nodes/ConcatExpr.hpp"
#include "regex/nodes/NodeVisitor.hpp"
#include "regex/nodes/OptionalExpr.hpp"
#include "regex/nodes/PlusClosure.hpp"
#include "regex/nodes/StarClosure.hpp"
#include "regex/nodes/SymbolAtom.hpp"

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
            void visit(const SymbolAtom& n) override {
                if(n.value()) {
                    out_ << '\'' << n.value().text() << '\'';
                } else {
                    out_ << "()";
                }
            }

            void visit(const OptionalExpr& n) override {
                n.inner()->accept(*this);
                out_ << '?';
            }

            void visit(const PlusClosure& n) override {
                n.inner()->accept(*this);
                out_ << '+';
            }

            void visit(const StarClosure& n) override {
                n.inner()->accept(*this);
                out_ << '*';
            }

            void visit(const AlternExpr& n) override {
                out_ << '(';
                n.left()->accept(*this);
                out_ << " | ";
                n.right()->accept(*this);
                out_ << ')';
            }

            void visit(const ConcatExpr& n) override {
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


    void SymbolAtom::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void StarClosure::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void PlusClosure::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void OptionalExpr::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void ConcatExpr::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void AlternExpr::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }
}
