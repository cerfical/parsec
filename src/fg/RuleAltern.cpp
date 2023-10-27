#include "fg/RuleAltern.hpp"
#include "fg/RuleTraverser.hpp"

namespace parsec::fg {
	void RuleAltern::traverse(RuleTraverser& traverser) const {
		traverser.visit(*this);
	}
}