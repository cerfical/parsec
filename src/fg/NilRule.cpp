#include "fg/NilRule.hpp"
#include "fg/RuleVisitor.hpp"

namespace parsec::fg {
	void NilRule::acceptVisitor(RuleVisitor& visitor) const {
		visitor.visit(*this);
	}
}