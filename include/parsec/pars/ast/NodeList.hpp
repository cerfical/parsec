#ifndef PARSEC_PARS_AST_NODE_LIST_HEADER
#define PARSEC_PARS_AST_NODE_LIST_HEADER

#include "Node.hpp"

namespace parsec::pars::ast {

	/**
	 * @brief Combines multiple Node%s into a single list.
	*/
	class NodeList : public Node {
	public:

		NodeList(NodePtr last, NodePtr rest)
			: last(std::move(last)), rest(std::move(rest)) {
		}

		~NodeList() override = default;


		void acceptVisitor(NodeVisitor& visitor) const override;


		NodePtr last;
		NodePtr rest;
	};

}

#endif