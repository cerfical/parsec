#ifndef PARSEC_PARS_AST_EMPTY_NODE_HEADER
#define PARSEC_PARS_AST_EMPTY_NODE_HEADER

#include "Node.hpp"

namespace parsec::pars {

	/**
	 * @brief Empty node.
	*/
	class EmptyNode : public Node {
	public:

		void accept(NodeVisitor& visitor) const override;

	};

}

#endif