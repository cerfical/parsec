#include "fg/rules/NilRule.hpp"
#include "fg/rules/RuleTraverser.hpp"

namespace parsec::fg::rules {
	void NilRule::traverseWith(RuleTraverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool NilRule::nullable() const noexcept {
		return true;
	}
}