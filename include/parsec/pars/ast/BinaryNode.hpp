#ifndef PARSEC_PARS_AST_BINARY_NODE_HEADER
#define PARSEC_PARS_AST_BINARY_NODE_HEADER

#include "Node.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Node with two child nodes.
	*/
	class BinaryNode : public Node {
	public:

		BinaryNode(NodePtr left, NodePtr right)
			: left(std::move(left)), right(std::move(right))
		{ }

		~BinaryNode() override = default;
		

		NodePtr left;
		NodePtr right;
	};

}

#endif