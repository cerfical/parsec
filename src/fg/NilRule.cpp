#include "fg/NilRule.hpp"
#include "fg/RuleTraverser.hpp"

namespace parsec::fg {
	void NilRule::traverse(RuleTraverser& traverser) const {
		traverser.visit(*this);
	}
}