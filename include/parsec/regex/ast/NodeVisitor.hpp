#pragma once

namespace parsec::regex {

    class SymbolAtom;

    class StarClosure;
    class PlusClosure;
    class OptionalExpr;

    class AlternExpr;
    class ConcatExpr;

    class NodeVisitor {
    public:

        virtual void visit(const SymbolAtom& n) = 0;

        virtual void visit(const StarClosure& n) = 0;
        virtual void visit(const PlusClosure& n) = 0;
        virtual void visit(const OptionalExpr& n) = 0;

        virtual void visit(const AlternExpr& n) = 0;
        virtual void visit(const ConcatExpr& n) = 0;

    protected:
        ~NodeVisitor() = default;
    };

}
