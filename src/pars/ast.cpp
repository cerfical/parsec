#include "pars/ast.hpp"

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