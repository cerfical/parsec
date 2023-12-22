#include "regex/nodes/ConcatExpr.hpp"
#include "regex/nodes/NodeVisitor.hpp"

namespace parsec::regex::nodes {
	void ConcatExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}