#include "fg/RuleConcat.hpp"
#include "fg/RuleTraverser.hpp"

namespace parsec::fg {
	void RuleConcat::traverse(RuleTraverser& traverser) const {
		traverser.visit(*this);
	}
}