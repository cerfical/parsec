#include "fg/SyntaxNode.hpp"
#include "fg/NodePrinter.hpp"

namespace parsec::fg {
	std::ostream& operator<<(std::ostream& out, const SyntaxNode& n) {
		NodePrinter(out).print(n);
		return out;
	}
}