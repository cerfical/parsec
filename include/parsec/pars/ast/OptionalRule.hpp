#ifndef PARSEC_PARS_AST_OPTIONAL_RULE_HEADER
#define PARSEC_PARS_AST_OPTIONAL_RULE_HEADER

#include "UnaryNode.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Makes another rule 'optional', in the sense that it is either present or not.
	*/
	class OptionalRule : public UnaryNode {
	public:

		explicit OptionalRule(NodePtr inner)
			: UnaryNode(std::move(inner))
		{ }

		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif