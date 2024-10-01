#pragma once

namespace parsec::pars {

    class EmptyNode;
    class ListNode;

    class InlineToken;
    class NamedToken;
    class NamedRule;

    class SymbolAtom;
    class EmptyRule;
    class ConcatRule;
    class AlternRule;
    class OptionalRule;
    class PlusRule;
    class StarRule;

    /**
     * @brief Implements Visitor design pattern for Node%s.
     */
    class NodeVisitor {
    public:

        virtual void visit(const EmptyNode& n) = 0;
        virtual void visit(const ListNode& n) = 0;

        virtual void visit(const InlineToken& n) = 0;
        virtual void visit(const NamedToken& n) = 0;
        virtual void visit(const NamedRule& n) = 0;

        virtual void visit(const SymbolAtom& n) = 0;
        virtual void visit(const EmptyRule& n) = 0;
        virtual void visit(const ConcatRule& n) = 0;
        virtual void visit(const AlternRule& n) = 0;
        virtual void visit(const OptionalRule& n) = 0;
        virtual void visit(const PlusRule& n) = 0;
        virtual void visit(const StarRule& n) = 0;

    protected:
        ~NodeVisitor() = default;
    };

}
