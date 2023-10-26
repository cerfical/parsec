#ifndef PARSEC_FG_RULES_RULE_ALTERN_HEADER
#define PARSEC_FG_RULES_RULE_ALTERN_HEADER

#include "BinaryRule.hpp"

namespace parsec::fg::rules {
	class RuleAltern : public BinaryRule {
	public:
		/** @{ */
		RuleAltern(std::unique_ptr<Rule> left, std::unique_ptr<Rule> right) noexcept
			: BinaryRule(std::move(left), std::move(right))
		{ }
		/** @} */


		/** @{ */
		void traverse(RuleTraverser& traverser) const override;
		/** @} */
	};
}

#endif