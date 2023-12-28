#include "pars/TokenKinds.hpp"

namespace parsec::pars {
	std::ostream& operator<<(std::ostream& out, TokenKinds tok) {
		switch(tok) {

#define PARSEC_PARS_TOKEN_KIND(tok, comment) case TokenKinds::tok: out << #tok; break;

			PARSEC_PARS_TOKEN_KIND_LIST

#undef PARSEC_PARS_TOKEN_KIND
		}

		return out;
	}

	void TokenKindVisitor::visit(TokenKinds tok) {
		switch(tok) {

#define PARSEC_PARS_TOKEN_KIND(tok, comment) case TokenKinds::tok: on##tok(); break;

			PARSEC_PARS_TOKEN_KIND_LIST
		
#undef PARSEC_PARS_TOKEN_KIND

		}
	}
}