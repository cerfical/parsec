#include "regex/ConcatExpr.hpp"
#include "regex/NodeVisitor.hpp"

namespace parsec::regex {
	void ConcatExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}