#include "fg/TokenRule.hpp"

namespace parsec::fg {
	void TokenRule::print(std::ostream& out) const {
		out << name() << " = \"" << m_pattern.toStr() << '"';
	}
}