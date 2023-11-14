#include "fg/RuleConcat.hpp"
#include "fg/RuleVisitor.hpp"

namespace parsec::fg {
	void RuleConcat::acceptVisitor(RuleVisitor& visitor) const {
		visitor.visit(*this);
	}
}