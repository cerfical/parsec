#include "fg/rules/RuleRef.hpp"
#include "fg/rules/RuleTraverser.hpp"

namespace parsec::fg::rules {
	void RuleRef::traverseWith(RuleTraverser& traverser) const {
		traverser.visitNode(*this);
	}
}