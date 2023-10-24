#ifndef PARSEC_FG_RULES_RULE_CONCAT_HEADER
#define PARSEC_FG_RULES_RULE_CONCAT_HEADER

#include "BinaryRule.hpp"

namespace parsec::fg::rules {
	class RuleConcat : public BinaryRule {
	public:
		/** @{ */
		RuleConcat(std::unique_ptr<RuleNode> left, std::unique_ptr<RuleNode> right) noexcept
			: BinaryRule(std::move(left), std::move(right))
		{ }
		/** @} */


		/** @{ */
		void traverseWith(RuleTraverser& traverser) const override;
		bool nullable() const noexcept override;
		/** @} */
	};
}

#endif