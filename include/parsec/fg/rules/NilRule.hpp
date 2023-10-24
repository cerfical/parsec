#ifndef PARSEC_FG_RULES_NIL_RULE_HEADER
#define PARSEC_FG_RULES_NIL_RULE_HEADER

#include "RuleNode.hpp"

namespace parsec::fg::rules {
	class NilRule : public RuleNode {
	public:
		/** @{ */
		void traverseWith(RuleTraverser& traverser) const override;
		bool nullable() const noexcept override;
		/** @} */
	};
}

#endif