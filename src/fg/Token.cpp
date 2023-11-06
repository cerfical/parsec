#include "fg/Token.hpp"

namespace parsec::fg {
	std::ostream& operator<<(std::ostream& out, const Token& tok) {
		out << "{ " << tok.kind() << ": \"" << tok.text() << "\" }";
		return out;
	}
}