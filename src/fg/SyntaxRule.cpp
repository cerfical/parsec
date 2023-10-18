#include "fg/SyntaxRule.hpp"

namespace parsec::fg {
	void SyntaxRule::print(std::ostream& out) const {
		out << name() << " = ";
		m_body->print(out);
	}
}