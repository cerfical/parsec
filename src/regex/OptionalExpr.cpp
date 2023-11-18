#include "regex/OptionalExpr.hpp"
#include "regex/NodeVisitor.hpp"

namespace parsec::regex {
	void OptionalExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}