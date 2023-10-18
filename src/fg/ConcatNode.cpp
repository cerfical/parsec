#include "fg/ConcatNode.hpp"
#include "fg/NodeTraverser.hpp"

namespace parsec::fg {
	void ConcatNode::traverseWith(NodeTraverser& traverser) const {
		traverser.visitNode(*this);
	}
}