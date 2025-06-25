module;

#include <memory>
#include <string>

export module parsec.regex:ast;

export import :ast.ExprNode;
export import :ast.NodeVisitor;
export import :ast.UnaryExprNode;
export import :ast.BinaryExprNode;

export import :ast.AtomExprNode;

export import :ast.AlternExprNode;
export import :ast.ConcatExprNode;

export import :ast.StarClosureNode;
export import :ast.PlusClosureNode;
export import :ast.OptionalExprNode;

import parsec.text;

namespace parsec::regex {

    /** @{ */
    /** @name Regular expression construction */
    /**
     * @brief Create a AtomExprNode.
     */
    export NodePtr atom(std::string value) {
        return std::make_shared<AtomExprNode>(std::move(value));
    }


    /**
     * @brief Create an empty AtomExprNode.
     */
    export NodePtr empty() {
        return atom("");
    }


    /**
     * @brief Create an AlternExprNode.
     */
    export NodePtr altern(NodePtr left, NodePtr right) {
        return std::make_shared<AlternExprNode>(left, right);
    }


    /**
     * @brief Create a ConcatExprNode.
     */
    export NodePtr concat(NodePtr left, NodePtr right) {
        return std::make_shared<ConcatExprNode>(left, right);
    }


    /**
     * @brief Create a StarClosureNode.
     */
    export NodePtr starClosure(NodePtr inner) {
        return std::make_shared<StarClosureNode>(inner);
    }


    /**
     * @brief Create a PlusClosureNode.
     */
    export NodePtr plusClosure(NodePtr inner) {
        return std::make_shared<PlusClosureNode>(inner);
    }


    /**
     * @brief Create an OptionalExprNode.
     */
    export NodePtr optional(NodePtr inner) {
        return std::make_shared<OptionalExprNode>(inner);
    }
    /** @} */


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
                if(!n.isNull()) {
                    out_ << '\'' << text::escape(n.value()) << '\'';
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
