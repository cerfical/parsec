#ifndef PARSEC_PARS_AST_ALTERN_RULE_HEADER
#define PARSEC_PARS_AST_ALTERN_RULE_HEADER

#include "BinaryNode.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Rule alternating between two rules.
	*/
	class AlternRule : public BinaryNode {
	public:
			
		AlternRule(NodePtr left, NodePtr right)
			: BinaryNode(std::move(left), std::move(right))
		{ }

		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif