#include "regex/make.hpp"
#include "regex/nodes.hpp"

namespace parsec::regex {
	NodePtr atom(const Symbol& value) {
		return std::make_shared<SymbolAtom>(value);
	}


	NodePtr altern(NodePtr left, NodePtr right) {
		if(left && right) {
			return std::make_shared<AlternExpr>(left, right);
		}
		return left ? left : right;
	}


	NodePtr concat(NodePtr left, NodePtr right) {
		if(left && right) {
			return std::make_shared<ConcatExpr>(left, right);
		}
		return left ? left : right;
	}


	NodePtr starClosure(NodePtr inner) {
		return inner ? std::make_shared<StarClosure>(inner) : inner;
	}


	NodePtr plusClosure(NodePtr inner) {
		return inner ? std::make_shared<PlusClosure>(inner) : inner;
	}


	NodePtr optional(NodePtr inner) {
		return inner ? std::make_shared<OptionalExpr>(inner) : inner;
	}
}