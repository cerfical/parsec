#include "regex/nodes/OptionalExpr.hpp"
#include "regex/nodes/NodeVisitor.hpp"

namespace parsec::regex::nodes {
	void OptionalExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}