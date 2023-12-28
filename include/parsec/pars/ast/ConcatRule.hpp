#ifndef PARSEC_PARS_AST_CONCAT_RULE_HEADER
#define PARSEC_PARS_AST_CONCAT_RULE_HEADER

#include "BinaryNode.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Rule appending one rule after another.
	*/
	class ConcatRule : public BinaryNode {
	public:
			
		ConcatRule(NodePtr left, NodePtr right)
			: BinaryNode(std::move(left), std::move(right))
		{ }

		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif