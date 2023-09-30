#include "BnfToken.hpp"

namespace parsec {
	std::ostream& operator<<(std::ostream& out, BnfTokenKinds tok) {
		switch(tok) {
#define PARSEC_BNF_TOKEN(tok) case BnfTokenKinds::tok: { out << #tok; break; }

			PARSEC_BNF_TOKEN_LIST

#undef PARSEC_BNF_TOKEN
		}
		return out;
	}

	std::ostream& operator<<(std::ostream& out, const BnfToken& tok) {
		out << '(' << tok.GetKind();
		if(!tok.GetText().empty()) {
			out <<  ", \"" << tok.GetText() << "\"";
		}
		out << ')';
		return out;
	}
}