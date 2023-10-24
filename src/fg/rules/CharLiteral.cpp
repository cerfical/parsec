#include "fg/rules/CharLiteral.hpp"
#include "fg/rules/RuleTraverser.hpp"

namespace parsec::fg::rules {
	void CharLiteral::traverseWith(RuleTraverser& traverser) const {
		traverser.visitNode(*this);
	}
}