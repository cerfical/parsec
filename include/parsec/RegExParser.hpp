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
		/** @brief Construct a RegExParser. */
		RegExParser() = default;

		/** @brief Destroy RegExParser. */
		~RegExParser() = default;
		/** @} */

		/** @{ */
		/** @brief Construct a RegExParser from a moved copy of RegExParser object. */
		RegExParser(RegExParser&&) = default;

		/** @brief Assign a moved copy of RegExParser object to RegExParser. */
		RegExParser& operator=(RegExParser&&) = default;
		/** @} */

		/** @{ */
		RegExParser(const RegExParser&) = delete;
		RegExParser& operator=(const RegExParser&) = delete;
		/** @} */

		/** @{ */
		/** @brief Analyse a string for a valid regular expression. */
		std::unique_ptr<RegExNode> Parse(std::string_view regex) {
			input = regex; pos = 0;
			ParseRegex();
			return std::move(this->regex);
		}
		/** @} */

	private:
		/** @brief Get position of RegExParser in input string. */
		SourceLocation GetInputPos() const noexcept;
		/** @brief Check if input is empty. */
		bool IsInputEmpty() const noexcept;
		
		/** @brief Return next character from input without removing it. */
		char PeekChar() const noexcept; 
		/** @brief Remove next character from input and return it. */
		char GetChar() noexcept;
		
		/** @brief Remove next character only if it is equal to the given one. */
		bool SkipCharIf(char ch) noexcept;
		/** @brief Remove next character from input. */
		void SkipChar() noexcept;

		/** @brief Check if next character starts an atom. */
		bool IsAtomStart() const noexcept;

		/** @brief Parse an atom. */
		void ParseAtom();
		/** @brief Parse a star expression. */
		void ParseStar();
		/** @brief Parse a concatenation. */
		void ParseConcat();
		/** @brief Parse an alternation. */
		void ParseAltern();
		/** @brief Parse a regular expression. */
		void ParseRegex();

		std::unique_ptr<RegExNode> regex;

		std::string_view input;
		gsl::index pos = { };
	};
}

#endif