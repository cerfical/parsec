#include "pars/ast.hpp"

namespace parsec::pars::ast {
	void NilNode::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void NodeList::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
	

	void InlinePattern::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void NamedPattern::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
	
	
	void NilRule::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
	
	void NameRule::acceptVisitor(NodeVisitor& visitor) const {
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