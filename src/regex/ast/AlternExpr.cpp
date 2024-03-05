#include "regex/ast/AlternExpr.hpp"
#include "regex/ast/NodeVisitor.hpp"

namespace parsec::regex::ast {
	void AlternExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}