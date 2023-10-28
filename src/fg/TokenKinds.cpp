#include "fg/TokenKinds.hpp"

namespace parsec::fg {
	std::ostream& operator<<(std::ostream& out, TokenKinds tok) {
		switch(tok) {
#define PARSEC_FG_TOKEN_KIND(kind, desc) \
			case TokenKinds::kind: out << #kind; break;
			
			PARSEC_FG_TOKEN_KIND_LIST

#undef PARSEC_FG_TOKEN_KIND

			default: out << "UnknownToken"; break;
		}
		return out;
	}

	std::string_view describeToken(TokenKinds tok) {
		switch(tok) {
#define PARSEC_FG_TOKEN_KIND(kind, desc) \
			case TokenKinds::kind: return desc;
			
			PARSEC_FG_TOKEN_KIND_LIST

#undef PARSEC_FG_TOKEN_KIND
			
			default: return "unknown token";
		}
	}
}