#ifndef PARSEC_FG_RULES_RULE_POS_HEADER
#define PARSEC_FG_RULES_RULE_POS_HEADER

#include "RuleNode.hpp"

namespace parsec::fg::rules {
	class RulePos : public RuleNode {
	public:
		/** @{ */
		bool nullable() const noexcept override;
		/** @} */


		/** @{ */
		RulePosList followPos() const;
		/** @} */
	};
}

#endif