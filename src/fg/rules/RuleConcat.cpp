#include "fg/rules/RuleConcat.hpp"
#include "fg/rules/RuleTraverser.hpp"

namespace parsec::fg::rules {
	void RuleConcat::traverse(RuleTraverser& traverser) const {
		traverser.visit(*this);
	}
}