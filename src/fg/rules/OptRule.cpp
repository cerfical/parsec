#include "fg/rules/OptRule.hpp"
#include "fg/rules/RuleTraverser.hpp"

namespace parsec::fg::rules {
	void OptRule::traverseWith(RuleTraverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool OptRule::nullable() const noexcept {
		return true;
	}
}