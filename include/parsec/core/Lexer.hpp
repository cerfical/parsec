#ifndef PARSEC_LEXER_HEADER
#define PARSEC_LEXER_HEADER

#include "Token.hpp"
#include "LexerBase.hpp"

namespace parsec {
	/**
	 * @brief Breaks the input into @ref Token "tokens".
	 */
	class Lexer : public LexerBase<Token, TokenKinds> {
	public:
		using LexerBase::LexerBase;

		/** @{ */
		/** @brief Ensures that the next token is of the specified type and returns it. */
		Token expect(TokenKinds kind) {
			if(!peek().is(kind)) {
				unmatched(Token::describe(kind));
			}
			return lex();
		}
		
		/** @brief Ensures that the next token is of the specified type and skips it. */
		void match(TokenKinds kind) {
			if(!peek().is(kind)) {
				unmatched(Token::describe(kind));
			}
			skip();
		}
		/** @} */

	private:
		[[noreturn]] void unmatched(std::string_view expected) const;

		TokenKinds onTokenParse() const override;


		bool isPatternStart() const;
		bool isIdentStart() const;
		bool isWordStart() const;


		TokenKinds parseIdent() const;
		TokenKinds parsePattern() const;
		TokenKinds parseOperator() const;

		void skipWhitespace() const;
	};
}

#endif