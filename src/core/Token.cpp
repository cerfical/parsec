#include "core/Token.hpp"

namespace parsec {
	std::string_view Token::describe(TokenKinds tok) {
		switch(tok) {
#define PARSEC_TOKEN_KIND(kind, desc) \
			case TokenKinds::kind: return desc;

			PARSEC_TOKEN_KIND_LIST

#undef PARSEC_TOKEN_KIND

			default: return "an unknown token";
		}
	}
}