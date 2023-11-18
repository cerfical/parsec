#ifndef PARSEC_UTILS_TEXT_SCANNER_HEADER
#define PARSEC_UTILS_TEXT_SCANNER_HEADER

#include "SourceLoc.hpp"

#include <string_view>
#include <iostream>
#include <string>

namespace parsec {
	/**
	 * @brief Facilitates textual data analysis using standard stream classes.
	 */
	class TextScanner {
	public:
		/** @{ */
		/** @brief Construct a new scanner that operates on a specified @c std::istream. */
		TextScanner(std::istream& input = std::cin) noexcept
			: m_input(&input) {
			m_input->exceptions(
				std::ios::badbit | std::ios::failbit
			);
		}
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

		
		/** @brief Returns the character at the specified offset from the current position without removing it from the stream. */
		char peek(int i = 0) const;


		/** @brief Checks if the end of input has been reached. */
		bool isEof() const {
			return m_labuf.empty() && checkForEof();
		}
		/** @} */


		/** @{ */
		/** @brief Removes specified character from the beginning of the input. */
		bool skipIf(char ch);


		/** @brief Removes specified text from the beginning of the input. */
		bool skipIf(std::string_view text);


		/** @brief Remove the next character from the input. */
		void skip() {
			get();
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
		[[noreturn]] void unexpectedEof() const;


		/** @{ */
		bool checkForEof() const;
		void updateLoc(char ch);

		bool fillBuf(int size) const;
		/** @} */


		mutable std::string m_labuf;
		std::istream* m_input;

		int m_linePos = 0;
		int m_lineNo = 0;
		int m_pos = 0;
	};
}

#endif