#include "core/TokenKinds.hpp"

namespace parsec {
	std::ostream& operator<<(std::ostream& out, TokenKinds tok) {
		switch(tok) {
#define PARSEC_TOKEN_KIND(kind, desc) \
			case TokenKinds::kind: out << #kind; break;
			
			PARSEC_TOKEN_KIND_LIST

#undef PARSEC_TOKEN_KIND

			default: out << "UnknownToken"; break;
		}
		return out;
	}
}