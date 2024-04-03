#include "regex/make.hpp"
#include "regex/nodes.hpp"

namespace parsec::regex {
	NodePtr atom(const Symbol& value) {
		return std::make_shared<SymbolAtom>(value);
	}


	NodePtr altern(NodePtr left, NodePtr right) {
		return std::make_shared<AlternExpr>(left, right);
	}


	NodePtr concat(NodePtr left, NodePtr right) {
		return std::make_shared<ConcatExpr>(left, right);
	}


	NodePtr starClosure(NodePtr inner) {
		return std::make_shared<StarClosure>(inner);
	}


	NodePtr plusClosure(NodePtr inner) {
		return std::make_shared<PlusClosure>(inner);
	}


	NodePtr optional(NodePtr inner) {
		return std::make_shared<OptionalExpr>(inner);
	}
}