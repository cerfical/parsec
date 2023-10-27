#include "fg/Token.hpp"

namespace parsec::fg {
	std::ostream& operator<<(std::ostream& out, const Token& tok) {
		out << '<' << tok.kind();
		if(!tok.text().empty()) {
			out <<  ", \"" << tok.text() << '\"';
		}
		out << '>';
		return out;
	}
}