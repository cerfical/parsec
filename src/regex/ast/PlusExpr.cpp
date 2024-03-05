#include "regex/ast/PlusExpr.hpp"
#include "regex/ast/NodeVisitor.hpp"

namespace parsec::regex::ast {
	void PlusExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}