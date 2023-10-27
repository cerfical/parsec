#ifndef PARSEC_FG_RULE_CONCAT_HEADER
#define PARSEC_FG_RULE_CONCAT_HEADER

#include "BinaryRule.hpp"

namespace parsec::fg {
	class RuleConcat : public BinaryRule {
	public:
		/** @{ */
		RuleConcat(RulePtr left, RulePtr right) noexcept
			: BinaryRule(std::move(left), std::move(right))
		{ }
		/** @} */


		/** @{ */
		void traverse(RuleTraverser& traverser) const override;
		/** @} */
	};
}

#endif