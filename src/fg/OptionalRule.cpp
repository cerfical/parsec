#include "fg/OptionalRule.hpp"
#include "fg/RuleTraverser.hpp"

namespace parsec::fg {
	void OptionalRule::traverse(RuleTraverser& traverser) const {
		traverser.visit(*this);
	}
}