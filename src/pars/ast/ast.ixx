export module parsec.pars:ast;

export import :ast.Node;
export import :ast.NodeVisitor;
export import :ast.UnaryRuleNode;
export import :ast.BinaryRuleNode;

export import :ast.EmptyNode;
export import :ast.ListNode;

export import :ast.InlineTokenNode;
export import :ast.NamedTokenNode;
export import :ast.NamedRuleNode;

export import :ast.AlternRuleNode;
export import :ast.ConcatRuleNode;
export import :ast.EmptyRuleNode;
export import :ast.OptionalRuleNode;
export import :ast.PlusRuleNode;
export import :ast.StarRuleNode;
export import :ast.SymbolRuleNode;

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
