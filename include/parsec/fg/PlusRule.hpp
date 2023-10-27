#ifndef PARSEC_FG_PLUS_RULE_HEADER
#define PARSEC_FG_PLUS_RULE_HEADER

#include "UnaryRule.hpp"

namespace parsec::fg {
	class PlusRule : public UnaryRule {
	public:
		/** @{ */
		explicit PlusRule(RulePtr inner) noexcept
			: UnaryRule(std::move(inner))
		{ }
		/** @} */


		/** @{ */
		void traverse(RuleTraverser& traverser) const override;
		/** @} */
	};
}

#endif