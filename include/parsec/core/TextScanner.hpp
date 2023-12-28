#ifndef PARSEC_TEXT_SCANNER_HEADER
#define PARSEC_TEXT_SCANNER_HEADER

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
		TextScanner() = default;

		TextScanner(std::istream& input)
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

		
		/** @brief Returns the character at the specified offset from the current position without removing it from the stream. */

		char peek(gsl::index i = 0) const;

		/** @brief Checks if the end of input has been reached. */
		bool isEof() const {
			return m_labuf.empty() && checkForEof();
		}
		/** @} */


		/** @{ */
		/** @brief Removes specified character from the input. */
		bool skipIf(char ch);


		/** @brief Removes specified text from the input. */
		bool skipIf(std::string_view text);


		/** @brief Remove the next character from the input. */
		void skip() {
			get();
		}
		/** @} */


		/** @{ */
		/**
		 * @brief The line where the scanner is positioned.
		*/
		const LineInfo& line() const noexcept {
			return m_line;
		}



		/**
		 * @brief Position of the scanner in the input stream.
		*/
		gsl::index pos() const noexcept {
			return m_pos;
		}


		/** @brief All information about the location of the scanner in a compact form. */
		SourceLoc loc() const noexcept {
			return SourceLoc(line(), pos());
		}
		/** @} */


	private:
		[[noreturn]] void unexpectedEof() const;


		/** @{ */
		bool checkForEof() const;
		void updateLoc(char ch);

		bool fillBuf(std::size_t size) const;
		/** @} */


		mutable std::string m_labuf;
		std::istream* m_input = {};

		gsl::index m_pos = 0;
		LineInfo m_line;
	};
}

#endif