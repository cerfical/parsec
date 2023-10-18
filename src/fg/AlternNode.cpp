#include "fg/AlternNode.hpp"
#include "fg/NodeTraverser.hpp"

namespace parsec::fg {
	void AlternNode::traverseWith(NodeTraverser& traverser) const {
		traverser.visitNode(*this);
	}
}