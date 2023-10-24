#include "fg/rules/PlusRule.hpp"
#include "fg/rules/RuleTraverser.hpp"

namespace parsec::fg::rules {
	void PlusRule::traverseWith(RuleTraverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool PlusRule::nullable() const noexcept {
		return false;
	}
}