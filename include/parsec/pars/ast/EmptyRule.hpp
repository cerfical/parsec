#ifndef PARSEC_PARS_AST_EMPTY_RULE_HEADER
#define PARSEC_PARS_AST_EMPTY_RULE_HEADER

#include "Node.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Rule matching empty string.
	*/
	class EmptyRule : public Node {
	public:

		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif