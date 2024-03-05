#include "regex/ast/ConcatExpr.hpp"
#include "regex/ast/NodeVisitor.hpp"

namespace parsec::regex::ast {
	void ConcatExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}