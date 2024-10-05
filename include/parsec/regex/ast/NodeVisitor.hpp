#pragma once

namespace parsec::regex {

    class SymbolAtom;

    class StarClosure;
    class PlusClosure;
    class OptionalExpr;

    class AlternExpr;
    class ConcatExpr;

    /**
     * @brief Implements a visitor for ExprNode%s.
     */
    class NodeVisitor {
    public:

        /**
         * @brief Called for a SymbolAtom.
         */
        virtual void visit(const SymbolAtom& n) = 0;


        /**
         * @brief Called for a StarClosure.
         */
        virtual void visit(const StarClosure& n) = 0;


        /**
         * @brief Called for a PlusClosure.
         */
        virtual void visit(const PlusClosure& n) = 0;


        /**
         * @brief Called for an OptionalExpr.
         */
        virtual void visit(const OptionalExpr& n) = 0;


        /**
         * @brief Called for an AlternExpr.
         */
        virtual void visit(const AlternExpr& n) = 0;


        /**
         * @brief Called for a ConcatExpr.
         */
        virtual void visit(const ConcatExpr& n) = 0;


    protected:
        ~NodeVisitor() = default;
    };

}
