#include "regex/PlusExpr.hpp"
#include "regex/NodeTraverser.hpp"

namespace parsec::regex {
	void PlusExpr::traverseWith(NodeTraverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool PlusExpr::nullable() const noexcept {
		return false;
	}
}