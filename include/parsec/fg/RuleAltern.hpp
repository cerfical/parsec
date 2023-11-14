#ifndef PARSEC_FG_RULE_ALTERN_HEADER
#define PARSEC_FG_RULE_ALTERN_HEADER

#include "BinaryRule.hpp"

namespace parsec::fg {
	class RuleAltern : public BinaryRule {
	public:
		/** @{ */
		RuleAltern(RulePtr left, RulePtr right) noexcept
			: BinaryRule(std::move(left), std::move(right))
		{ }
		/** @} */

		/** @{ */
		void acceptVisitor(RuleVisitor& visitor) const override;
		/** @} */
	};
}

#endif