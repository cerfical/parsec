#pragma once

#include "../core/Symbol.hpp"
#include "nodes/ExprNode.hpp"

namespace parsec::regex {

	NodePtr atom(const Symbol& value);


	inline NodePtr empty() {
		return atom("");
	}


	NodePtr altern(NodePtr left, NodePtr right);


	NodePtr concat(NodePtr left, NodePtr right);


	NodePtr starClosure(NodePtr inner);


	NodePtr plusClosure(NodePtr inner);


	NodePtr optional(NodePtr inner);

}
