#include "fg/rules/StarRule.hpp"
#include "fg/rules/RuleTraverser.hpp"

namespace parsec::fg::rules {
	void StarRule::traverse(RuleTraverser& traverser) const {
		traverser.visit(*this);
	}
}