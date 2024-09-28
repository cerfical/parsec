#include "pars/ast/AlternRule.hpp"
#include "pars/ast/ConcatRule.hpp"
#include "pars/ast/EmptyNode.hpp"
#include "pars/ast/EmptyRule.hpp"
#include "pars/ast/InlineToken.hpp"
#include "pars/ast/ListNode.hpp"
#include "pars/ast/NamedRule.hpp"
#include "pars/ast/NamedToken.hpp"
#include "pars/ast/NodeVisitor.hpp"
#include "pars/ast/OptionalRule.hpp"
#include "pars/ast/PlusRule.hpp"
#include "pars/ast/StarRule.hpp"
#include "pars/ast/SymbolAtom.hpp"

namespace parsec::pars {
    void EmptyNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void ListNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }


    void InlineToken::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void NamedToken::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }


    void EmptyRule::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void SymbolAtom::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void NamedRule::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }


    void ConcatRule::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void AlternRule::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }


    void OptionalRule::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void PlusRule::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void StarRule::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }
}
