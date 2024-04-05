#ifndef PARSEC_PARS_AST_STAR_RULE_HEADER
#define PARSEC_PARS_AST_STAR_RULE_HEADER

#include "UnaryRule.hpp"

namespace parsec::pars {

	/**
	 * @brief Denotes an application of the Kleene star operator to a rule.
	*/
	class StarRule : public UnaryRule {
	public:

		using UnaryRule::UnaryRule;

		void accept(NodeVisitor& visitor) const override;

	};

}

#endif