#ifndef PARSEC_PARS_AST_OPTIONAL_RULE_HEADER
#define PARSEC_PARS_AST_OPTIONAL_RULE_HEADER

#include "UnaryRule.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Denotes an optional rule.
	*/
	class OptionalRule : public UnaryRule {
	public:

		using UnaryRule::UnaryRule;

		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif