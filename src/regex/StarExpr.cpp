#include "regex/StarExpr.hpp"
#include "regex/NodeVisitor.hpp"

namespace parsec::regex {
	void StarExpr::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}