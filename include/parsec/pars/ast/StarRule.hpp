#ifndef PARSEC_PARS_AST_STAR_RULE_HEADER
#define PARSEC_PARS_AST_STAR_RULE_HEADER

#include "UnaryNode.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Repeats another rule zero or more times.
	*/
	class StarRule : public UnaryNode {
	public:

		explicit StarRule(NodePtr inner)
			: UnaryNode(std::move(inner))
		{ }

		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif