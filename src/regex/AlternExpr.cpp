#include "regex/AlternExpr.hpp"
#include "regex/NodeTraverser.hpp"

namespace parsec::regex {
	void AlternExpr::traverseWith(NodeTraverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool AlternExpr::nullable() const noexcept {
		return left()->nullable() || right()->nullable();
	}
}