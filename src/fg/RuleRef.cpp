#include "fg/RuleRef.hpp"
#include "fg/NodeTraverser.hpp"

namespace parsec::fg {
	void RuleRef::traverseWith(NodeTraverser& traverser) const {
		traverser.visitNode(*this);
	}
}