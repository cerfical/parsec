#include "fg/rules/OptRule.hpp"
#include "fg/rules/RuleTraverser.hpp"

namespace parsec::fg::rules {
	void OptRule::traverse(RuleTraverser& traverser) const {
		traverser.visit(*this);
	}
}