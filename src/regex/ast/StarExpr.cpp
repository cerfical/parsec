#include "regex/ast/StarExpr.hpp"
#include "regex/ast/NodeVisitor.hpp"

namespace parsec::regex::ast {
	void StarExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}