#include "regex/nodes/CharAtom.hpp"
#include "regex/nodes/NodeVisitor.hpp"

namespace parsec::regex::nodes {
	void CharAtom::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}