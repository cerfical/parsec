#include "Pattern.hpp"
#include <gsl/narrow>

namespace parsec {
	CharAtom* Pattern::addAtom(char value) {
		auto atom = &m_atoms.emplace_back(
			value, gsl::narrow_cast<int>(m_atoms.size())
		);
		return atom;
	}
}