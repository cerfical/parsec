#ifndef PARSEC_REGEX_HEADER
#define PARSEC_REGEX_HEADER

#include "regex/SymbolAtom.hpp"
#include "regex/OptionalExpr.hpp"
#include "regex/StarClosure.hpp"
#include "regex/PlusClosure.hpp"
#include "regex/ConcatExpr.hpp"
#include "regex/AlternExpr.hpp"
#include "regex/NodeVisitor.hpp"

#include "regex/Parser.hpp"

namespace parsec::regex {

	inline NodePtr atom(const Symbol& value) {
		return std::make_shared<SymbolAtom>(value);
	}


	inline NodePtr empty() {
		return atom("");
	}


	inline NodePtr altern(NodePtr left, NodePtr right) {
		return std::make_shared<AlternExpr>(left, right);
	}


	inline NodePtr concat(NodePtr left, NodePtr right) {
		return std::make_shared<ConcatExpr>(left, right);
	}


	inline NodePtr starClosure(NodePtr inner) {
		return std::make_shared<StarClosure>(inner);
	}


	inline NodePtr plusClosure(NodePtr inner) {
		return std::make_shared<PlusClosure>(inner);
	}


	inline NodePtr optional(NodePtr inner) {
		return std::make_shared<OptionalExpr>(inner);
	}

}

#endif