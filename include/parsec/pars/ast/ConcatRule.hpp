#ifndef PARSEC_PARS_AST_CONCAT_RULE_HEADER
#define PARSEC_PARS_AST_CONCAT_RULE_HEADER

#include "BinaryRule.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Concatenation of two rules.
	*/
	class ConcatRule : public BinaryRule {
	public:

		using BinaryRule::BinaryRule;

		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif