#include "regex/ast/CharAtom.hpp"
#include "regex/ast/NodeVisitor.hpp"

namespace parsec::regex::ast {
	void CharAtom::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}