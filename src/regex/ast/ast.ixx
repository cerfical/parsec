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

}
