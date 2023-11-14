#ifndef PARSEC_FG_OPTIONAL_RULE_HEADER
#define PARSEC_FG_OPTIONAL_RULE_HEADER

#include "UnaryRule.hpp"

namespace parsec::fg {
	class OptionalRule : public UnaryRule {
	public:
		/** @{ */
		explicit OptionalRule(RulePtr inner) noexcept
			: UnaryRule(std::move(inner))
		{ }
		/** @} */

		/** @{ */
		void acceptVisitor(RuleVisitor& visitor) const override;
		/** @} */
	};
}

#endif