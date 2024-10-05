#pragma once

#include "../core/Symbol.hpp"
#include "ast/ExprNode.hpp"

namespace parsec::regex {

    /**
     * @{
     * @name Convenience factory functions to construct regular expressions.
     */

    /**
     * @brief Create a SymbolAtom.
     */
    NodePtr atom(const Symbol& value);


    /**
     * @brief Create an empty SymbolAtom.
     */
    inline NodePtr empty() {
        return atom("");
    }


    /**
     * @brief Create an AlternExpr.
     */
    NodePtr altern(NodePtr left, NodePtr right);


    /**
     * @brief Create a ConcatExpr.
     */
    NodePtr concat(NodePtr left, NodePtr right);


    /**
     * @brief Create a StarClosure.
     */
    NodePtr starClosure(NodePtr inner);


    /**
     * @brief Create a PlusClosure.
     */
    NodePtr plusClosure(NodePtr inner);


    /**
     * @brief Create an OptionalExpr.
     */
    NodePtr optional(NodePtr inner);
    /** @} */

}
