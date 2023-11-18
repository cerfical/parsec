#include "regex/NilExpr.hpp"
#include "regex/NodeVisitor.hpp"

namespace parsec::regex {
	void NilExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}