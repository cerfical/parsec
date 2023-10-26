#ifndef PARSEC_FG_RULES_RULE_CONCAT_HEADER
#define PARSEC_FG_RULES_RULE_CONCAT_HEADER

#include "BinaryRule.hpp"

namespace parsec::fg::rules {
	class RuleConcat : public BinaryRule {
	public:
		/** @{ */
		RuleConcat(std::unique_ptr<Rule> left, std::unique_ptr<Rule> right) noexcept
			: BinaryRule(std::move(left), std::move(right))
		{ }
		/** @} */


		/** @{ */
		void traverse(RuleTraverser& traverser) const override;
		/** @} */
	};
}

#endif