#include "fg/PlusRule.hpp"
#include "fg/RuleVisitor.hpp"

namespace parsec::fg {
	void PlusRule::acceptVisitor(RuleVisitor& visitor) const {
		visitor.visit(*this);
	}
}