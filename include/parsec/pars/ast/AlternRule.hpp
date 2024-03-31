#ifndef PARSEC_PARS_AST_ALTERN_RULE_HEADER
#define PARSEC_PARS_AST_ALTERN_RULE_HEADER

#include "BinaryRule.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Alternation of two rules.
	*/
	class AlternRule : public BinaryRule {
	public:

		using BinaryRule::BinaryRule;

		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif