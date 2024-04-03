#include "regex/nodes.hpp"
#include "regex/NodeVisitor.hpp"

namespace parsec::regex {
	void SymbolAtom::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void StarClosure::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void PlusClosure::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void OptionalExpr::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void ConcatExpr::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void AlternExpr::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}