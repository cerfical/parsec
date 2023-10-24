#include "fg/rules/StarRule.hpp"
#include "fg/rules/RuleTraverser.hpp"

namespace parsec::fg::rules {
	void StarRule::traverseWith(RuleTraverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool StarRule::nullable() const noexcept {
		return true;
	}
}