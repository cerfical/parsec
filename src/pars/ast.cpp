#include "pars/ast/AlternRuleNode.hpp"
#include "pars/ast/ConcatRuleNode.hpp"
#include "pars/ast/EmptyNode.hpp"
#include "pars/ast/EmptyRuleNode.hpp"
#include "pars/ast/InlineTokenNode.hpp"
#include "pars/ast/ListNode.hpp"
#include "pars/ast/NamedRuleNode.hpp"
#include "pars/ast/NamedTokenNode.hpp"
#include "pars/ast/NodeVisitor.hpp"
#include "pars/ast/OptionalRuleNode.hpp"
#include "pars/ast/PlusRuleNode.hpp"
#include "pars/ast/StarRuleNode.hpp"
#include "pars/ast/SymbolRuleNode.hpp"

namespace parsec::pars {
    void EmptyNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void ListNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }


    void InlineTokenNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void NamedTokenNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }


    void EmptyRuleNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void SymbolRuleNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void NamedRuleNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }


    void ConcatRuleNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void AlternRuleNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }


    void OptionalRuleNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void PlusRuleNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void StarRuleNode::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }
}
