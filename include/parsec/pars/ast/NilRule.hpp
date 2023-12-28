#ifndef PARSEC_PARS_AST_NIL_RULE_HEADER
#define PARSEC_PARS_AST_NIL_RULE_HEADER

#include "Node.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Empty rule.
	*/
	class NilRule : public Node {
	public:

		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif