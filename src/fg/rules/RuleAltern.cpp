#include "fg/rules/RuleAltern.hpp"
#include "fg/rules/RuleTraverser.hpp"

namespace parsec::fg::rules {
	void RuleAltern::traverse(RuleTraverser& traverser) const {
		traverser.visit(*this);
	}
}