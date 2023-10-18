#include "regex/OptionalExpr.hpp"
#include "regex/NodeTraverser.hpp"

namespace parsec::regex {
	void OptionalExpr::traverseWith(NodeTraverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool OptionalExpr::nullable() const noexcept {
		return true;
	}
}