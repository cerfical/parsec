#include "fg/PlusRule.hpp"
#include "fg/RuleTraverser.hpp"

namespace parsec::fg {
	void PlusRule::traverse(RuleTraverser& traverser) const {
		traverser.visit(*this);
	}
}