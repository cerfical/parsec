#ifndef PARSEC_PARS_AST_UNARY_NODE_HEADER
#define PARSEC_PARS_AST_UNARY_NODE_HEADER

#include "Node.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Node with one child node.
	*/
	class UnaryNode : public Node {
	public:

		explicit UnaryNode(NodePtr inner)
			: inner(std::move(inner))
		{ }

		~UnaryNode() override = default;


		NodePtr inner;
	};

}

#endif