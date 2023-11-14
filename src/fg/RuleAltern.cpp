#include "fg/RuleAltern.hpp"
#include "fg/RuleVisitor.hpp"

namespace parsec::fg {
	void RuleAltern::acceptVisitor(RuleVisitor& visitor) const {
		visitor.visit(*this);
	}
}