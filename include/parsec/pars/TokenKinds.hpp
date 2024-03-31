#ifndef PARSEC_PARS_TOKEN_KINDS_HEADER
#define PARSEC_PARS_TOKEN_KINDS_HEADER

#define PARSEC_PARS_TOKEN_KIND_LIST \
	PARSEC_PARS_TOKEN_KIND(Nil, Empty token) \
	PARSEC_PARS_TOKEN_KIND(Eof, End of file) \
\
	PARSEC_PARS_TOKEN_KIND(Ident, Identifier) \
	PARSEC_PARS_TOKEN_KIND(PatternString, Regex string pattern) \
	PARSEC_PARS_TOKEN_KIND(RawString, Raw string literal) \
\
	PARSEC_PARS_TOKEN_KIND(Star, Asterisk symbol) \
	PARSEC_PARS_TOKEN_KIND(Plus, Plus symbol) \
	PARSEC_PARS_TOKEN_KIND(Qo, Question mark) \
	PARSEC_PARS_TOKEN_KIND(Pipe, Vertical bar) \
\
	PARSEC_PARS_TOKEN_KIND(Semicolon, Semicolon) \
	PARSEC_PARS_TOKEN_KIND(Equals, Equals sign) \
\
	PARSEC_PARS_TOKEN_KIND(LeftBrace, Opening curly brace) \
	PARSEC_PARS_TOKEN_KIND(RightBrace, Closing curly brace) \
\
	PARSEC_PARS_TOKEN_KIND(LeftParen, Opening parenthesis) \
	PARSEC_PARS_TOKEN_KIND(RightParen, Closing parenthesis)

namespace parsec::pars {

#define PARSEC_PARS_TOKEN_KIND(tok, comment) /** @brief comment##. */ tok,

	/**
	 * @brief List of token types produced by @ref parsec::pars::Lexer "Lexer".
	*/
	enum class TokenKinds {

		PARSEC_PARS_TOKEN_KIND_LIST

	};

#undef PARSEC_PARS_TOKEN_KIND

}

#endif