#ifndef PARSEC_PARS_AST_NIL_NODE_HEADER
#define PARSEC_PARS_AST_NIL_NODE_HEADER

#include "Node.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Empty node.
	*/
	class NilNode : public Node {
	public:

		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif