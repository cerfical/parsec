#ifndef PARSEC_FG_TOKEN_KINDS_HEADER
#define PARSEC_FG_TOKEN_KINDS_HEADER

#define PARSEC_FG_TOKEN_KIND_LIST \
	PARSEC_FG_TOKEN_KIND(Eof, "an end of file") /**< @brief End of file. */ \
\
	PARSEC_FG_TOKEN_KIND(Ident, "an identifier") /**< @brief Identifier. */ \
	PARSEC_FG_TOKEN_KIND(StringLiteral, "a string literal") /**< @brief String literal. */ \
\
	PARSEC_FG_TOKEN_KIND(Pipe, "a '|'") /**< @brief Vertical bar. */ \
	PARSEC_FG_TOKEN_KIND(Semicolon, "a ';'") /**< @brief Semicolon. */ \
	PARSEC_FG_TOKEN_KIND(Equals, "an '='") /**< @brief Equals sign. */ \
\
	PARSEC_FG_TOKEN_KIND(OpenBrace, "an '{'") /**< @brief Opening brace. */ \
	PARSEC_FG_TOKEN_KIND(CloseBrace, "a '}'") /**< @brief Closing brace. */ \
\
	PARSEC_FG_TOKEN_KIND(OpenParen, "an '('") /**< @brief Opening parenthesis. */ \
	PARSEC_FG_TOKEN_KIND(CloseParen, "a ')'") /**< @brief Closing parenthesis. */

#include <string>
#include <ostream>

namespace parsec::fg {
#define PARSEC_FG_TOKEN_KIND(tok, desc) tok,

	enum class TokenKinds {
		PARSEC_FG_TOKEN_KIND_LIST
	};

#undef PARSEC_FG_TOKEN_KIND


	std::ostream& operator<<(std::ostream& out, TokenKinds tok);

	std::string describeToken(TokenKinds tok);
}

#endif