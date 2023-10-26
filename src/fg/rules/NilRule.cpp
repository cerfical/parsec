#include "fg/rules/NilRule.hpp"
#include "fg/rules/RuleTraverser.hpp"

namespace parsec::fg::rules {
	void NilRule::traverse(RuleTraverser& traverser) const {
		traverser.visit(*this);
	}
}