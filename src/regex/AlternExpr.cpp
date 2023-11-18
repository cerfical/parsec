#include "regex/AlternExpr.hpp"
#include "regex/NodeVisitor.hpp"

namespace parsec::regex {
	void AlternExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}