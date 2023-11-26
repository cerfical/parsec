#ifndef PARSEC_TOKEN_KINDS_HEADER
#define PARSEC_TOKEN_KINDS_HEADER

#define PARSEC_TOKEN_KIND_LIST \
	PARSEC_TOKEN_KIND(None, "an empty token") /**< @brief Empty token. */ \
	PARSEC_TOKEN_KIND(Eof, "an end of file") /**< @brief End of file. */ \
\
	PARSEC_TOKEN_KIND(Ident, "an identifier") /**< @brief Identifier. */ \
	PARSEC_TOKEN_KIND(Pattern, "a string pattern") /**< @brief String pattern. */ \
\
	PARSEC_TOKEN_KIND(Star, "a '*'") /**< @brief Asterisk symbol. */ \
	PARSEC_TOKEN_KIND(Plus, "a '+'") /**< @brief Plus symbol. */ \
	PARSEC_TOKEN_KIND(Qo, "a '?'") /**< @brief Question mark. */ \
\
	PARSEC_TOKEN_KIND(Pipe, "a '|'") /**< @brief Vertical bar. */ \
	PARSEC_TOKEN_KIND(Semicolon, "a ';'") /**< @brief Semicolon. */ \
	PARSEC_TOKEN_KIND(Equals, "an '='") /**< @brief Equals sign. */ \
\
	PARSEC_TOKEN_KIND(LeftBrace, "a '{'") /**< @brief Opening brace. */ \
	PARSEC_TOKEN_KIND(RightBrace, "a '}'") /**< @brief Closing brace. */ \
\
	PARSEC_TOKEN_KIND(LeftParen, "a '('") /**< @brief Opening parenthesis. */ \
	PARSEC_TOKEN_KIND(RightParen, "a ')'") /**< @brief Closing parenthesis. */

#include <ostream>

namespace parsec {
#define PARSEC_TOKEN_KIND(tok, desc) tok,

	/**
	 * @brief Lists all available token types used in a textual grammar definition.
	 */
	enum class TokenKinds {
		PARSEC_TOKEN_KIND_LIST
	};

#undef PARSEC_TOKEN_KIND


	std::ostream& operator<<(std::ostream& out, TokenKinds tok);
}

#endif