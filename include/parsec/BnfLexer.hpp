#ifndef PARSEC_BNF_LEXER_HEADER
#define PARSEC_BNF_LEXER_HEADER

#include "BnfToken.hpp"

#include <string_view>
#include <optional>
#include <istream>

namespace parsec {
	/**
	 * @brief Breaks down a character stream into a sequence of @ref BnfToken "BnfTokens".
	 */
	class BnfLexer {
	public:
		/** @{ */
		/** @brief Construct a defaulted lexer that does nothing. */
		BnfLexer() = default;

		/** @brief Construct a new lexer that operates on a @c std::istream. */
		explicit BnfLexer(std::istream* input) noexcept
		 : m_input(input)
		{ }
		/** @} */

		/** @{ */
		BnfLexer(BnfLexer&&) = default;
		BnfLexer& operator=(BnfLexer&&) = default;
		/** @} */

		/** @{ */
		BnfLexer(const BnfLexer&) = delete;
		BnfLexer& operator=(const BnfLexer&) = delete;
		/** @} */

		/** @{ */
		/** @brief Current position of the lexer in the input. */
		const SourceLoc& GetInputPos() const noexcept {
			return m_pos;
		}
		/** @} */

		/** @{ */
		/** @brief Check if the next token is the specified keyword. */
		bool IsKeyword(std::string_view keyword);
		/** @brief Check if the end of input was reached. */
		bool IsEof();
		/** @} */

		/** @{ */
		/** @brief Look at the next token to be extracted, parsing it if necessary. */
		const BnfToken& Peek();

		/** @brief Extract the next token from the input stream. */
		BnfToken Lex();
		/** @} */

	private:
		/** @{ */
		[[noreturn]] void Error(gsl::czstring msg);

		[[noreturn]] void UnexpectedChar();
		[[noreturn]] void UnexpectedEof();
		/** @} */

		/** @{ */
		char PeekChar();
		bool SkipCharIf(char ch);
		void SkipChar();
		char GetChar();
		/** @} */

		/** @{ */
		void SkipWhitespace();

		bool IsIdentStart();
		BnfTokenKinds ParseIdent();

		bool IsRegexStart();
		BnfTokenKinds ParseRegex();
		/** @} */

		/** @{ */
		BnfTokenKinds ParseOperator();
		BnfTokenKinds ParseToken();
		/** @} */

		std::istream *m_input = nullptr;
		SourceLoc m_pos;
		std::string m_buf;

		std::optional<BnfToken> m_tok;
	};
}

#endif