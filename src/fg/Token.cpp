#include "fg/Token.hpp"
#include <sstream>

namespace parsec::fg {
	std::ostream& operator<<(std::ostream& out, Token::Kinds kind) {
		switch(kind) {
#define PARSEC_FG_TOKEN(tok) case Token::tok: { out << #tok; break; }

			PARSEC_FG_TOKEN_LIST

#undef PARSEC_FG_TOKEN
		}
		return out;
	}

	std::ostream& operator<<(std::ostream& out, const Token& tok) {
		out << '(' << tok.kind();
		if(!tok.text().empty()) {
			out <<  ", \"" << tok.text() << '\"';
		}
		out << ')';
		return out;
	}


	std::string Token::toStr() const {
		return (std::ostringstream() << *this).str();
	}
}