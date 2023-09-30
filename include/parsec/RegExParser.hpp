#ifndef PARSEC_REGEX_PARSER_HEADER
#define PARSEC_REGEX_PARSER_HEADER

#include "RegExNodes.hpp"
#include "Utils.hpp"

#include <string_view>
#include <memory>

namespace parsec {
	/**
	 * @brief Parses regular expressions.
	 */
	class RegExParser {
	public:
		/** @name Construction/destruction: */

		/** @{ */
		RegExParser() = default;
		~RegExParser() = default;
		/** @} */

		/** @name Copy/move operations: */

		/** @{ */
		RegExParser(RegExParser&&) = default;
		RegExParser& operator=(RegExParser&&) = default;
		/** @} */

		/** @name Ordinary functions: */
		
		/** @{ */
		/** @brief Parses a string for a valid regular expression. */
		std::unique_ptr<RegExNode> Parse(std::string_view regex) {
			input = regex; pos = 0;
			ParseRegex();
			return std::move(this->regex);
		}
		/** @} */

	private:
		RegExParser(const RegExParser&) = delete;
		RegExParser& operator=(const RegExParser&) = delete;

		/** @brief Returns the current position of the parser in the input string. */
		SourceLocation GetCurrentLocation() const noexcept {
			const auto loc = SourceLocation({
				.chars = CharRange({ .startPos = pos, .size = 1 }),
				.lineNo = 0
			});
			return loc;
		}
		
		/** @brief Checks if the input is empty. */
		bool IsInputEmpty() const noexcept;
		
		char PeekChar() const noexcept; 
		/** @brief Removes the next character from the input and returns it. */
		char GetChar() noexcept;
		
		/** @brief Skips the character only if it is equal to the given one. */
		bool SkipCharIf(char ch) noexcept;
		/** @brief Removes the next character from the input. */
		void SkipChar() noexcept;

		/** @brief Checks if the next input character starts an atom. */
		bool IsAtomStart() const noexcept;

		/** @brief Parses an atom. */
		void ParseAtom();
		/** @brief Parses a star expression. */
		void ParseStar();
		/** @brief Parses a concatenation expression. */
		void ParseConcat();
		/** @brief Parses a alternation expression. */
		void ParseAltern();
		/** @brief Parses a regular expression. */
		void ParseRegex();

		std::unique_ptr<RegExNode> regex;

		std::string_view input;
		Index pos = { };
	};
}

#endif