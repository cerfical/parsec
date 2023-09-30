#ifndef PARSEC_LEXER_HEADER
#define PARSEC_LEXER_HEADER

#include "Token.hpp"
#include "Utils.hpp"

#include <gsl/util>

#include <istream>
#include <string>

namespace parsec {
	/**
	 * @brief Breaks down a character stream into a sequence of @ref Token "Token"s.
	 */
	class Lexer {
	public:
		/** @{ */
		/** @brief Construct a Lexer that operates on @c std::istream. */
		explicit Lexer(std::istream& input) noexcept
		 : input(&input)
		{ }
		
		/** @brief Destroy Lexer. */
		~Lexer() = default;
		/** @} */

		/** @{ */
		/** @brief Construct a Lexer from a moved copy of Lexer object. */
		Lexer(Lexer&&) = default;
		
		/** @brief Assign to Lexer a moved copy of Lexer object. */
		Lexer& operator=(Lexer&&) = default;
		/** @} */

		/** @{ */
		Lexer(const Lexer&) = delete;
		Lexer& operator=(const Lexer&) = delete;
		/** @} */

		/** @{ */
		/** @brief Extract next Token from Lexer input stream. */
		Token Lex();
		/** @} */

	private:
		/** @brief Construct a SourceLocation for Token currently being preccessed. */
		SourceLocation MakeTokenLocation() const noexcept;
		/** @brief Get position of Lexer in input stream. */
		SourceLocation GetInputPos() const noexcept;

		/** @brief Check if the end of input was reached. */
		bool IsInputEnd() const;

		/** @brief Return next character from input without removing it. */
		char PeekChar() const;
		/** @brief Remove next character from input. */
		void SkipChar();
		/** @brief Remove next character from input and return it. */
		char GetChar();

		/** @brief Remove characters from input until a non-whitespace character is found. */
		void SkipWhitespace();

		/** @brief Check if next input characters start a valid identifier. */
		bool IsIdentStart() const;
		/** @brief Parse an identifier token. */
		void ParseIdent();

		/** @brief Check if next input characters start a valid regular expression. */
		bool IsRegexStart() const;
		/** @brief Parse a regular expression. */
		void ParseRegex();

		/** @brief Parse an operator. */
		void ParseOperator();

		std::istream *input;
		
		TokenKinds tokKind = TokenKinds::Eof;
		std::string tokText;

		gsl::index lineNo = 0, colNo = 0;
		gsl::index pos = 0;
	};
}

#endif