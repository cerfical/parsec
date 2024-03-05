#include "regex/ast/NilExpr.hpp"
#include "regex/ast/NodeVisitor.hpp"

namespace parsec::regex::ast {
	void NilExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}