#pragma once

#include "ast/AlternExprNode.hpp"
#include "ast/AtomExprNode.hpp"
#include "ast/ConcatExprNode.hpp"
#include "ast/NodeVisitor.hpp"
#include "ast/OptionalExprNode.hpp"
#include "ast/PlusClosureNode.hpp"
#include "ast/StarClosureNode.hpp"

namespace parsec::regex {

    /** @{ */
    /** @name Regular expression construction */
    /**
     * @brief Create a AtomExprNode.
     */
    inline NodePtr atom(const Symbol& value) {
        return std::make_shared<AtomExprNode>(value);
    }


    /**
     * @brief Create an empty AtomExprNode.
     */
    inline NodePtr empty() {
        return atom("");
    }


    /**
     * @brief Create an AlternExprNode.
     */
    inline NodePtr altern(NodePtr left, NodePtr right) {
        return std::make_shared<AlternExprNode>(left, right);
    }


    /**
     * @brief Create a ConcatExprNode.
     */
    inline NodePtr concat(NodePtr left, NodePtr right) {
        return std::make_shared<ConcatExprNode>(left, right);
    }


    /**
     * @brief Create a StarClosureNode.
     */
    inline NodePtr starClosure(NodePtr inner) {
        return std::make_shared<StarClosureNode>(inner);
    }


    /**
     * @brief Create a PlusClosureNode.
     */
    inline NodePtr plusClosure(NodePtr inner) {
        return std::make_shared<PlusClosureNode>(inner);
    }


    /**
     * @brief Create an OptionalExprNode.
     */
    inline NodePtr optional(NodePtr inner) {
        return std::make_shared<OptionalExprNode>(inner);
    }
    /** @} */

}
