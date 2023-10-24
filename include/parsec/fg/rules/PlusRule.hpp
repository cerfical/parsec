#ifndef PARSEC_FG_RULES_PLUS_RULE_HEADER
#define PARSEC_FG_RULES_PLUS_RULE_HEADER

#include "UnaryRule.hpp"

namespace parsec::fg::rules {
	class PlusRule : public UnaryRule {
	public:
		/** @{ */
		explicit PlusRule(std::unique_ptr<RuleNode> inner) noexcept
			: UnaryRule(std::move(inner))
		{ }
		/** @} */


		/** @{ */
		void traverseWith(RuleTraverser& traverser) const override;
		bool nullable() const noexcept override;
		/** @} */
	};
}

#endif