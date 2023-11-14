#include "fg/OptionalRule.hpp"
#include "fg/RuleVisitor.hpp"

namespace parsec::fg {
	void OptionalRule::acceptVisitor(RuleVisitor& visitor) const {
		visitor.visit(*this);
	}
}