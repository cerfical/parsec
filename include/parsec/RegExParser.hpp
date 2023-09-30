#ifndef PARSEC_REGEX_PARSER_HEADER
#define PARSEC_REGEX_PARSER_HEADER

#include "RegExNodes.hpp"
#include "Utils.hpp"

#include <gsl/gsl>

#include <string_view>
#include <memory>

namespace parsec {
	/**
	 * @brief Parses regular expressions.
	 */
	class RegExParser {
	public:
		/** @{ */
		/** @brief Construct a new RegExParser. */
		RegExParser() = default;

		/** @brief Destroy the RegExParser. */
		~RegExParser() = default;
		/** @} */

		/** @{ */
		/** @brief Construct a new RegExParser by moving from another RegExParser. */
		RegExParser(RegExParser&&) = default;

		/** @brief Move another RegExParser into the RegExParser. */
		RegExParser& operator=(RegExParser&&) = default;
		/** @} */

		/** @{ */
		RegExParser(const RegExParser&) = delete;
		RegExParser& operator=(const RegExParser&) = delete;
		/** @} */

		/** @{ */
		/** @brief Analyse a string for a valid regular expression. */
		std::unique_ptr<RegExNode> Parse(std::string_view regex);
		/** @} */

	private:
		/** @brief Get the current position of the RegExParser in the input. */
		SourceLocation GetInputPos() const noexcept;
		/** @brief Check if the input is empty. */
		bool IsInputEmpty() const noexcept;

		/** @brief Return the next character from the input without removing it. */
		char PeekChar() const noexcept; 
		/** @brief Remove the next character from the input and return it. */
		char GetChar() noexcept;
		
		/** @brief Remove the next character only if it is equal to the given one. */
		bool SkipCharIf(char ch) noexcept;
		/** @brief Remove the next character from the input. */
		void SkipChar() noexcept;

		/** @brief Check if the next character starts an atom. */
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
		gsl::index pos = 0;
	};
}

#endif