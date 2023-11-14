#ifndef PARSEC_FG_STAR_RULE_HEADER
#define PARSEC_FG_STAR_RULE_HEADER

#include "UnaryRule.hpp"

namespace parsec::fg {
	class StarRule : public UnaryRule {
	public:
		/** @{ */
		explicit StarRule(RulePtr inner) noexcept
			: UnaryRule(std::move(inner))
		{ }
		/** @} */

		/** @{ */
		void acceptVisitor(RuleVisitor& visitor) const override;
		/** @} */
	};
}

#endif