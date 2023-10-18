#include "regex/StarExpr.hpp"
#include "regex/NodeTraverser.hpp"

namespace parsec::regex {
	void StarExpr::traverseWith(NodeTraverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool StarExpr::nullable() const noexcept {
		return true;
	}
}