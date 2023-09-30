#ifndef PARSEC_BNF_LEXER_HEADER
#define PARSEC_BNF_LEXER_HEADER

#include "BnfToken.hpp"
#include "Utils.hpp"

#include <gsl/util>

#include <istream>
#include <string>

namespace parsec {
	/**
	 * @brief Breaks down a character stream into a sequence of @ref BnfToken "BnfTokens".
	 */
	class BnfLexer {
	public:
		/** @{ */
		/** @brief Construct a new lexer that operates on @c std::istream. */
		explicit BnfLexer(std::istream& input) noexcept
		 : input(&input)
		{ }
		
		/** @copybrief */
		~BnfLexer() = default;
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
		/** @brief Get the current position of the lexer in the input. */
		SourceLocation GetInputPos() const noexcept;

		/** @brief Determine the type of the next token to be extracted, parsing it if necessary. */
		BnfTokenKinds Peek();

		/** @brief Extract the next token from the input stream. */
		BnfToken Lex();
		/** @} */

	private:
		/** @brief Check if the end of input has been reached. */
		bool IsInputEnd() const;
		/** @brief Check if the next token has already been parsed. */
		bool IsTokenParsed() const noexcept;

		/** @brief Return the next character from the input without removing it. */
		char PeekChar() const;
		/** @brief Remove the next character from the input. */
		void SkipChar();
		/** @brief Remove the next character from the input and return it. */
		char GetChar();

		/** @brief Remove characters from the input until a non-whitespace character is found. */
		void SkipWhitespace();

		/** @brief Check if the input starts a valid identifier. */
		bool IsIdentStart() const;
		/** @brief Parse an identifier token. */
		void ParseIdent();

		/** @brief Check if the next input start a valid regular expression. */
		bool IsRegexStart() const;
		/** @brief Parse a regular expression. */
		void ParseRegex();

		/** @brief Parse an operator. */
		void ParseOperator();
		/** @brief Parse the next token from the input. */
		void ParseToken();

		std::istream *input;

		gsl::index lineStart = 0;
		gsl::index lineNo = 0, colNo = 0;
		std::size_t colCount = 0;
		
		BnfTokenKinds tokKind = BnfTokenKinds::Eof;
		std::string tokText;
	};
}

#endif