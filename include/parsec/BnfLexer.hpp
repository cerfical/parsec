#ifndef PARSEC_BNF_LEXER_HEADER
#define PARSEC_BNF_LEXER_HEADER

#include "BndToken.hpp"
#include "Utils.hpp"

#include <gsl/util>

#include <istream>
#include <string>

namespace parsec {
	/**
	 * @brief Breaks down a character stream into a sequence of @ref BnfToken "BnfToken"s.
	 */
	class BnfLexer {
	public:
		/** @{ */
		/** @brief Construct a new BnfLexer that operates on @c std::istream. */
		explicit BnfLexer(std::istream& input) noexcept
		 : input(&input)
		{ }
		
		/** @brief Destroy the BnfLexer. */
		~BnfLexer() = default;
		/** @} */

		/** @{ */
		/** @brief Construct a new BnfLexer by moving from another BnfLexer. */
		BnfLexer(BnfLexer&&) = default;
		
		/** @brief Move another BnfLexer to the BnfLexer. */
		BnfLexer& operator=(BnfLexer&&) = default;
		/** @} */

		/** @{ */
		BnfLexer(const BnfLexer&) = delete;
		BnfLexer& operator=(const BnfLexer&) = delete;
		/** @} */

		/** @{ */
		/** @brief Get the current position of the BnfLexer in the input. */
		SourceLocation GetInputPos() const noexcept;

		/** @brief Determine the type of the next BnfToken to be extracted, parsing it if necessary. */
		BnfTokenKinds Peek();

		/** @brief Extract the next BnfToken from the input stream. */
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