#include "stringify.hpp"

#include "pars/TokenKinds.hpp"
#include "pars/Token.hpp"

#include <format>

namespace parsec {
	std::string stringify(pars::TokenKinds tok) {
		switch(tok) {

#define PARSEC_PARS_TOKEN_KIND(tok, comment) case pars::TokenKinds::tok: return #tok;
			
			PARSEC_PARS_TOKEN_KIND_LIST

#undef PARSEC_PARS_TOKEN_KIND

		}
		return "UnknownTokenType";
	}


	std::string stringify(const pars::Token& tok) {
		return std::format("({}: \"{}\")", stringify(tok.kind()), tok.text());
	}
}