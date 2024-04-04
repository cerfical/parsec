#ifndef PARSEC_PARS_AST_PLUS_RULE_HEADER
#define PARSEC_PARS_AST_PLUS_RULE_HEADER

#include "UnaryRule.hpp"

namespace parsec::pars {

	/**
	 * @brief Denotes an application of the Kleene plus operator to a rule.
	*/
	class PlusRule : public UnaryRule {
	public:

		using UnaryRule::UnaryRule;

		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif