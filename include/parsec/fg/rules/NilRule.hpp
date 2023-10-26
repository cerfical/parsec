#ifndef PARSEC_FG_RULES_NIL_RULE_HEADER
#define PARSEC_FG_RULES_NIL_RULE_HEADER

#include "Rule.hpp"

namespace parsec::fg::rules {
	class NilRule : public Rule {
	public:
		/** @{ */
		void traverse(RuleTraverser& traverser) const override;
		/** @} */
	};
}

#endif