#include "pars/ast.hpp"

namespace parsec::pars::ast {
	void EmptyNode::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void ListNode::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
	

	void InlineToken::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void NamedToken::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
	
	
	void EmptyRule::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
	
	void SymbolRule::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void NamedRule::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
	

	void ConcatRule::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void AlternRule::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}


	void OptionalRule::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void PlusRule::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void StarRule::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}