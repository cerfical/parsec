module parsec.pars;

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
