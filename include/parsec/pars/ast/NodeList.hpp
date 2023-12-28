#ifndef PARSEC_PARS_AST_NODE_LIST_HEADER
#define PARSEC_PARS_AST_NODE_LIST_HEADER

#include "Node.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Combines multiple Node%s into a single list.
	*/
	class NodeList : public Node {
	public:

		NodeList(NodePtr first, NodePtr rest)
			: first(std::move(first)), rest(std::move(rest))
		{ }

		~NodeList() override = default;


		void acceptVisitor(NodeVisitor& visitor) const override;


		NodePtr first;
		NodePtr rest;
	};

}

#endif