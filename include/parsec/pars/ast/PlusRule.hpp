#ifndef PARSEC_PARS_AST_PLUS_RULE_HEADER
#define PARSEC_PARS_AST_PLUS_RULE_HEADER

#include "UnaryNode.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Repeats another rule one or more times.
	*/
	class PlusRule : public UnaryNode {
	public:

		explicit PlusRule(NodePtr inner)
			: UnaryNode(std::move(inner))
		{ }

		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif