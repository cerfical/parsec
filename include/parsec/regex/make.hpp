#pragma once

#include "ast/AlternExpr.hpp"
#include "ast/ConcatExpr.hpp"
#include "ast/NodeVisitor.hpp"
#include "ast/OptionalExpr.hpp"
#include "ast/PlusClosure.hpp"
#include "ast/StarClosure.hpp"
#include "ast/SymbolAtom.hpp"

namespace parsec::regex {

    /**
     * @{
     * @name Convenience factory functions to construct regular expressions.
     */

    /**
     * @brief Create a SymbolAtom.
     */
    inline NodePtr atom(Symbol value) {
        return std::make_shared<SymbolAtom>(std::move(value));
    }


    /**
     * @brief Create an empty SymbolAtom.
     */
    inline NodePtr empty() {
        return atom("");
    }


    /**
     * @brief Create an AlternExpr.
     */
    inline NodePtr altern(NodePtr left, NodePtr right) {
        if(left && right) {
            return std::make_shared<AlternExpr>(left, right);
        }
        return left ? left : right;
    }


    /**
     * @brief Create a ConcatExpr.
     */
    inline NodePtr concat(NodePtr left, NodePtr right) {
        if(left && right) {
            return std::make_shared<ConcatExpr>(left, right);
        }
        return left ? left : right;
    }


    /**
     * @brief Create a StarClosure.
     */
    inline NodePtr starClosure(NodePtr inner) {
        return inner ? std::make_shared<StarClosure>(inner) : inner;
    }


    /**
     * @brief Create a PlusClosure.
     */
    inline NodePtr plusClosure(NodePtr inner) {
        return inner ? std::make_shared<PlusClosure>(inner) : inner;
    }


    /**
     * @brief Create an OptionalExpr.
     */
    inline NodePtr optional(NodePtr inner) {
        return inner ? std::make_shared<OptionalExpr>(inner) : inner;
    }
    /** @} */

}
