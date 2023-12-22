#include "regex/nodes/NilExpr.hpp"
#include "regex/nodes/NodeVisitor.hpp"

namespace parsec::regex::nodes {
	void NilExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}