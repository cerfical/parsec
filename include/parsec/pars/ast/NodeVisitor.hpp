#pragma once

namespace parsec::pars {

    class EmptyNode;
    class ListNode;

    class InlineTokenNode;
    class NamedTokenNode;
    class NamedRuleNode;

    class SymbolRuleNode;
    class EmptyRuleNode;
    class ConcatRuleNode;
    class AlternRuleNode;
    class OptionalRuleNode;
    class PlusRuleNode;
    class StarRuleNode;

    /**
     * @brief Implements a visitor for Node%s.
     */
    class NodeVisitor {
    public:

        /**
         * @brief Called for an EmptyNode.
         */
        virtual void visit(const EmptyNode& n) = 0;


        /**
         * @brief Called for a ListNode.
         */
        virtual void visit(const ListNode& n) = 0;


        /**
         * @brief Called for an InlineTokenNode.
         */
        virtual void visit(const InlineTokenNode& n) = 0;


        /**
         * @brief Called for a NamedTokenNode.
         */
        virtual void visit(const NamedTokenNode& n) = 0;


        /**
         * @brief Called for a NamedRuleNode.
         */
        virtual void visit(const NamedRuleNode& n) = 0;


        /**
         * @brief Called for a SymbolRuleNode.
         */
        virtual void visit(const SymbolRuleNode& n) = 0;


        /**
         * @brief Called for an EmptyRuleNode.
         */
        virtual void visit(const EmptyRuleNode& n) = 0;


        /**
         * @brief Called for a ConcatRuleNode.
         */
        virtual void visit(const ConcatRuleNode& n) = 0;


        /**
         * @brief Called for an AlternRuleNode.
         */
        virtual void visit(const AlternRuleNode& n) = 0;


        /**
         * @brief Called for an OptionalRuleNode.
         */
        virtual void visit(const OptionalRuleNode& n) = 0;


        /**
         * @brief Called for a PlusRuleNode.
         */
        virtual void visit(const PlusRuleNode& n) = 0;


        /**
         * @brief Called for a StarRuleNode.
         */
        virtual void visit(const StarRuleNode& n) = 0;


    protected:
        ~NodeVisitor() = default;
    };

}
