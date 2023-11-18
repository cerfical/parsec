#include "regex/PlusExpr.hpp"
#include "regex/NodeVisitor.hpp"

namespace parsec::regex {
	void PlusExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}