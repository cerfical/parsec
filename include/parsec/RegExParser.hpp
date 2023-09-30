#ifndef PARSEC_REGEX_PARSER_HEADER
#define PARSEC_REGEX_PARSER_HEADER

#include "RegExNodes.hpp"
#include "Utils.hpp"

#include <gsl/util>

#include <string_view>
#include <memory>

namespace parsec {
	/**
	 * @brief Parses regular expressions.
	 */
	class RegExParser {
	public:
		/** @{ */
		/** @copybrief */
		RegExParser() = default;

		/** @copybrief */
		~RegExParser() = default;
		/** @} */

		/** @{ */
		RegExParser(RegExParser&&) = default;
		RegExParser& operator=(RegExParser&&) = default;
		/** @} */

		/** @{ */
		RegExParser(const RegExParser&) = delete;
		RegExParser& operator=(const RegExParser&) = delete;
		/** @} */

		/** @{ */
		/** @brief Analyze a string for a valid regular expression. */
		std::unique_ptr<RegExNode> Parse(std::string_view regex);
		/** @} */

	private:
		/** @brief Called when a character is encountered in an unexpected place. */
		[[noreturn]] void UnexpectedChar() const;
		/** @brief Called when the end of input is unexpectedly reached. */
		[[noreturn]] void UnexpectedEof() const;

		/** @brief Get the current position of the parser in the input. */
		SourceLocation GetInputPos() const noexcept;
		/** @brief Check if the input is empty. */
		bool IsInputEmpty() const noexcept;

		/** @brief Return the next character from the input without removing it. */
		char PeekChar() const; 
		/** @brief Remove the next character from the input and return it. */
		char GetChar();
		
		/** @brief Remove the next character only if it is equal to the given one. */
		bool SkipCharIf(char ch) noexcept;
		/** @brief Remove the next character from the input. */
		void SkipChar();

		/** @brief Check if the next character starts an atom. */
		bool IsAtomStart() const noexcept;

		/** @brief Parse an escape sequence. */
		char ParseEscapeSeq();
		/** @brief Parse a literal character. */
		char ParseChar();
		/** @brief Parse a character range. */
		void ParseCharRange();
		/** @brief Parse a character set. */
		void ParseCharSet();

		/** @brief Parse an atom. */
		void ParseAtom();
		/** @brief Parse a repetition expression. */
		void ParseRepeat();
		/** @brief Parse a concatenation. */
		void ParseConcat();
		/** @brief Parse an alternation. */
		void ParseAltern();
		/** @brief Parse a regular expression. */
		void ParseRegex();

		std::unique_ptr<RegExNode> regex;

		std::string_view input;
		gsl::index pos = 0;
	};
}

#endif