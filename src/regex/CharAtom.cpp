#include "regex/CharAtom.hpp"
#include "regex/NodeVisitor.hpp"

namespace parsec::regex {
	void CharAtom::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}