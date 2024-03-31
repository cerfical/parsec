#include "print.hpp"
#include "stringify.hpp"

namespace parsec {
	std::ostream& operator<<(std::ostream& out, pars::TokenKinds tok) {
		return out << stringify(tok);
	}


	std::ostream& operator<<(std::ostream& out, const pars::Token& tok) {
		return out << stringify(tok);
	}
}