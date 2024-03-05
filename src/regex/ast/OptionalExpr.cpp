#include "regex/ast/OptionalExpr.hpp"
#include "regex/ast/NodeVisitor.hpp"

namespace parsec::regex::ast {
	void OptionalExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}