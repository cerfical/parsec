#include "regex/nodes/StarExpr.hpp"
#include "regex/nodes/NodeVisitor.hpp"

namespace parsec::regex::nodes {
	void StarExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}