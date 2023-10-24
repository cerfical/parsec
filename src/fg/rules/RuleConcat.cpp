#include "fg/rules/RuleConcat.hpp"
#include "fg/rules/RuleTraverser.hpp"

namespace parsec::fg::rules {
	void RuleConcat::traverseWith(RuleTraverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool RuleConcat::nullable() const noexcept {
		return left()->nullable() && right()->nullable();
	}
}