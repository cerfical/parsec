#include "fg/rules/PlusRule.hpp"
#include "fg/rules/RuleTraverser.hpp"

namespace parsec::fg::rules {
	void PlusRule::traverse(RuleTraverser& traverser) const {
		traverser.visit(*this);
	}
}