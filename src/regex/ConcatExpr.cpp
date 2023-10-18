#include "regex/ConcatExpr.hpp"
#include "regex/NodeTraverser.hpp"

namespace parsec::regex {
	void ConcatExpr::traverseWith(NodeTraverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool ConcatExpr::nullable() const noexcept {
		return left()->nullable() && right()->nullable();
	}
}