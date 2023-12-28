#include "pars/Token.hpp"

namespace parsec::pars {
	std::ostream& operator<<(std::ostream& out, const Token& tok) {
		out << "{ " << tok.kind() << ": '" << tok.text() << "' }";
		return out;
	}
}