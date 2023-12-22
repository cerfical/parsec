#include "regex/nodes/AlternExpr.hpp"
#include "regex/nodes/NodeVisitor.hpp"

namespace parsec::regex::nodes {
	void AlternExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}