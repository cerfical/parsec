#include "regex/NilExpr.hpp"
#include "regex/NodeTraverser.hpp"

namespace parsec::regex {
	void NilExpr::traverseWith(NodeTraverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool NilExpr::nullable() const noexcept {
		return true;
	}
}