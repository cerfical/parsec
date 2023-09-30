#ifndef PARSEC_REG_EX_PARSER_HEADER
#define PARSEC_REG_EX_PARSER_HEADER

#include "RegExNodes.hpp"
#include <string_view>

namespace parsec {
	/**
	 * @brief Parses regular expressions.
	 */
	class RegExParser {
	public:
		/** @{ */
		/** @brief Set up a new ready-for-work parser. */
		RegExParser() = default;
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
		/** @brief Parse a regular expression into its tree representation. */
		std::unique_ptr<RegExNode> Parse(std::string_view regex);
		/** @} */

	private:
		/** @{ */
		[[noreturn]] void UnexpectedChar() const;
		[[noreturn]] void UnexpectedEof() const;
		/** @} */

		/** @{ */
		SourceLoc GetInputPos() const noexcept;
		bool IsInputEmpty() const noexcept;
		/** @} */

		/** @{ */
		char PeekChar() const; 
		char GetChar();

		bool SkipCharIf(char ch) noexcept;
		void SkipChar();
		/** @} */

		/** @{ */
		bool IsAtomStart() const noexcept;
		/** @} */

		/** @{ */
		char ParseEscapeSeq();
		char ParseChar();
		void ParseCharRange();
		void ParseCharSet();

		void ParseAtom();
		void ParseRepeat();
		void ParseConcat();
		void ParseAltern();
		void ParseRegex();
		/** @} */

		std::unique_ptr<RegExNode> m_regex;

		std::string_view m_input;
		int m_pos = 0;
	};
}

#endif