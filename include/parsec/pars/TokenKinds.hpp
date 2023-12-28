#ifndef PARSEC_PARS_TOKEN_KINDS_HEADER
#define PARSEC_PARS_TOKEN_KINDS_HEADER

#define PARSEC_PARS_TOKEN_KIND_LIST \
	PARSEC_PARS_TOKEN_KIND(Nil, Empty token) \
	PARSEC_PARS_TOKEN_KIND(Eof, End of file) \
\
	PARSEC_PARS_TOKEN_KIND(Ident, Identifier) \
	PARSEC_PARS_TOKEN_KIND(Pattern, String pattern) \
\
	PARSEC_PARS_TOKEN_KIND(Star, Asterisk symbol) \
	PARSEC_PARS_TOKEN_KIND(Plus, Plus symbol) \
	PARSEC_PARS_TOKEN_KIND(Qo, Question mark) \
	PARSEC_PARS_TOKEN_KIND(Pipe, Vertical bar) \
\
	PARSEC_PARS_TOKEN_KIND(Semicolon, Semicolon) \
	PARSEC_PARS_TOKEN_KIND(Equals, Equals sign) \
\
	PARSEC_PARS_TOKEN_KIND(LeftBrace, Opening brace) \
	PARSEC_PARS_TOKEN_KIND(RightBrace, Closing brace) \
\
	PARSEC_PARS_TOKEN_KIND(LeftParen, Opening parenthesis) \
	PARSEC_PARS_TOKEN_KIND(RightParen, Closing parenthesis)

#include <ostream>

namespace parsec::pars {

#define PARSEC_PARS_TOKEN_KIND(tok, comment) tok, /**< @brief comment##. */

	/**
	 * @brief Available token types that are produced by @ref parsec::pars::Lexer "Lexer".
	*/
	enum class TokenKinds {
		PARSEC_PARS_TOKEN_KIND_LIST
	};

#undef PARSEC_PARS_TOKEN_KIND



	/**
	 * @brief Provides a safe way to perform computations conditioned on @ref TokenKinds values.
	*/
	class TokenKindVisitor {
	public:
		
		/**
		 * @brief Check the value and call a method with the appropriate name.
		*/
		void visit(TokenKinds tok);

	protected:
		~TokenKindVisitor() = default;


#define PARSEC_PARS_TOKEN_KIND(tok, comment) virtual void on##tok() = 0;

		PARSEC_PARS_TOKEN_KIND_LIST

#undef PARSEC_PARS_TOKEN_KIND

	};



	std::ostream& operator<<(std::ostream& out, TokenKinds tok);

}

#endif