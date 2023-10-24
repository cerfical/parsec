#ifndef PARSEC_FG_RULES_RULE_ALTERN_HEADER
#define PARSEC_FG_RULES_RULE_ALTERN_HEADER

#include "BinaryRule.hpp"

namespace parsec::fg::rules {
	class RuleAltern : public BinaryRule {
	public:
		/** @{ */
		RuleAltern(std::unique_ptr<RuleNode> left, std::unique_ptr<RuleNode> right) noexcept
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