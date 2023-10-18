#include "fg/SyntaxRule.hpp"

namespace parsec::fg {
	std::ostream& operator<<(std::ostream& out, const SyntaxRule& r) {
		out << r.name() << " = " << *r.body();
		return out;
	}
}