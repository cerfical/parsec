#ifndef PARSEC_UTILS_TEXT_SCANNER_HEADER
#define PARSEC_UTILS_TEXT_SCANNER_HEADER

#include "SourceLoc.hpp"
#include <iostream>

namespace parsec {
	/**
	 * @brief Auxiliary wrapper class for @c std::istream input streams.
	 */
	class TextScanner {
	public:
		/** @{ */
		/** @brief Construct a new scanner that operates on a @c std::istream. */
		explicit TextScanner(std::istream& input = std::cin) noexcept
			: m_input(&input)
		{ }
		/** @} */


		/** @{ */
		TextScanner(TextScanner&&) = default;
		TextScanner& operator=(TextScanner&&) = default;
		/** @} */

		/** @{ */
		TextScanner(const TextScanner&) = delete;
		TextScanner& operator=(const TextScanner&) = delete;
		/** @} */


		/** @{ */
		/** @brief Removes the next character from the input and returns it. */
		char get();


		/** @brief Returns the next character from the input without removing it. */
		char peek() const;
		/** @} */


		/** @{ */
		/** @brief Removes the next character only if it is equal to the given one. */
		bool skipIf(char ch);


		/** @brief Remove the next character from the input. */
		void skip() {
			get();
		}


		/** @brief Check if the end of input has been reached. */
		bool eof() const {
			return m_input->peek() == std::char_traits<char>::eof();
		}
		/** @} */


		/** @{ */
		/** @brief Current line number where the scanner is positioned. */
		int lineNo() const noexcept {
			return m_lineNo;
		}


		/** @brief Position of the current line in the input. */
		int linePos() const noexcept {
			return m_linePos;
		}


		/** @brief Absolute position of the scanner in the input stream. */
		int pos() const noexcept {
			return m_pos;
		}


		/** @brief All information about the location of the scanner in a compact form. */
		SourceLoc loc() const noexcept {
			return SourceLoc(
				pos() - linePos(),
				1,
				lineNo(),
				linePos()
			);
		}
		/** @} */


	private:
		/** @{ */
		[[noreturn]] void unexpectedEof() const;
		/** @} */


		/** @{ */
		std::istream* m_input;
		/** @} */


		/** @{ */
		int m_linePos = 0;
		int m_lineNo = 0;
		int m_pos = 0;
		/** @} */
	};
}

#endif