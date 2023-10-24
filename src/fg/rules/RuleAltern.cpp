#include "fg/rules/RuleAltern.hpp"
#include "fg/rules/RuleTraverser.hpp"

namespace parsec::fg::rules {
	void RuleAltern::traverseWith(RuleTraverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool RuleAltern::nullable() const noexcept {
		return left()->nullable() || right()->nullable();
	}
}