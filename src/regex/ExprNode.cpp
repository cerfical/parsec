#include "regex/ExprNode.hpp"

#include "regex/ExprPrinter.hpp"
#include "regex/nodes.hpp"

namespace parsec::regex {
	std::ostream& operator<<(std::ostream& out, const ExprNode& n) {
		ExprPrinter(out).print(n);
		return out;
	}
}