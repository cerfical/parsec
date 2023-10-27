#ifndef PARSEC_FG_NIL_RULE_HEADER
#define PARSEC_FG_NIL_RULE_HEADER

#include "Rule.hpp"

namespace parsec::fg {
	class NilRule : public Rule {
	public:
		/** @{ */
		void traverse(RuleTraverser& traverser) const override;
		/** @} */
	};
}

#endif