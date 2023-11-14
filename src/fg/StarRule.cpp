#include "fg/StarRule.hpp"
#include "fg/RuleVisitor.hpp"

namespace parsec::fg {
	void StarRule::acceptVisitor(RuleVisitor& visitor) const {
		visitor.visit(*this);
	}
}