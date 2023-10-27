#include "fg/StarRule.hpp"
#include "fg/RuleTraverser.hpp"

namespace parsec::fg {
	void StarRule::traverse(RuleTraverser& traverser) const {
		traverser.visit(*this);
	}
}