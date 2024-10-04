#include "regex/make.hpp"

#include "regex/ast/AlternExpr.hpp"
#include "regex/ast/ConcatExpr.hpp"
#include "regex/ast/NodeVisitor.hpp"
#include "regex/ast/OptionalExpr.hpp"
#include "regex/ast/PlusClosure.hpp"
#include "regex/ast/StarClosure.hpp"
#include "regex/ast/SymbolAtom.hpp"

namespace parsec::regex {
    NodePtr atom(const Symbol& value) {
        return std::make_shared<SymbolAtom>(value);
    }


    NodePtr altern(NodePtr left, NodePtr right) {
        if(left && right) {
            return std::make_shared<AlternExpr>(left, right);
        }
        return left ? left : right;
    }


    NodePtr concat(NodePtr left, NodePtr right) {
        if(left && right) {
            return std::make_shared<ConcatExpr>(left, right);
        }
        return left ? left : right;
    }


    NodePtr starClosure(NodePtr inner) {
        return inner ? std::make_shared<StarClosure>(inner) : inner;
    }


    NodePtr plusClosure(NodePtr inner) {
        return inner ? std::make_shared<PlusClosure>(inner) : inner;
    }


    NodePtr optional(NodePtr inner) {
        return inner ? std::make_shared<OptionalExpr>(inner) : inner;
    }
}
