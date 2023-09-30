#ifndef PARSEC_FG_LEXER_HEADER
#define PARSEC_FG_LEXER_HEADER

#include "../utils/TextScanner.hpp"
#include "Token.hpp"

#include <optional>
#include <gsl/gsl>

namespace parsec::fg {
	/**
	 * @brief Breaks down a character stream into a sequence of @ref Token "Tokens".
	 */
	class Lexer {
	public:
		/** @{ */
		/** @brief Construct a defaulted lexer that does nothing. */
		Lexer() = default;

		/** @brief Construct a new lexer that operates on a @c std::istream. */
		explicit Lexer(std::istream& input) noexcept
			: m_input(input)
		{ }
		/** @} */


		/** @{ */
		Lexer(Lexer&&) = default;
		Lexer& operator=(Lexer&&) = default;
		/** @} */

		/** @{ */
		Lexer(const Lexer&) = delete;
		Lexer& operator=(const Lexer&) = delete;
		/** @} */


		/** @{ */
		/** @brief Look at the next token to be extracted, parsing it if necessary. */
		const Token& peek() {
			return parseToken();
		}

		/** @brief Check if the end of input was reached. */
		bool eof() {
			return peek().eof();
		}
		/** @} */


		/** @{ */
		/** @brief Remove the next token from the input only if it is of the specified type. */
		bool skipIf(Token::Kinds kind);

		/** @brief Remove the next token from the input. */
		void skip() {
			lex();
		}

		/** @brief Extract the next token from the input stream. */
		Token lex();
		/** @} */

	private:
		/** @{ */
		[[noreturn]] void parseError(gsl::czstring msg);
		[[noreturn]] void badCharError();
		/** @} */

		/** @{ */
		void skipWhitespace();

		bool identStart();
		Token::Kinds parseIdent();

		bool stringLiteralStart();
		Token::Kinds parseStringLiteral();

		Token::Kinds parseOperator();
		/** @} */

		/** @{ */
		Token& parseToken();
		/** @} */
		
		std::optional<Token> m_tok;

		TextScanner m_input;
		std::string m_buf;
	};
}

#endif