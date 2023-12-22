#include "regex/nodes/PlusExpr.hpp"
#include "regex/nodes/NodeVisitor.hpp"

namespace parsec::regex::nodes {
	void PlusExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}