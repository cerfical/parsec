#ifndef PARSEC_UTILS_TEXT_SCANNER_HEADER
#define PARSEC_UTILS_TEXT_SCANNER_HEADER

#include <istream>

namespace parsec {
	/**
	 * @brief Auxiliary wrapper class for @c std::istream input streams.
	 */
	class TextScanner {
	public:
		/** @{ */
		/** @brief Construct a defaulted scanner that does nothing. */
		TextScanner() = default;

		/** @brief Construct a new scanner that operates on a @c std::istream. */
		explicit TextScanner(std::istream& input) noexcept
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
		/** @brief Check if the end of input was reached. */
		bool eof() const;

		/** @brief Return the next character from the input without removing it. */
		char peek() const;
		/** @} */


		/** @{ */
		/** @brief Remove the next character only if it is equal to the given one. */
		bool skipIf(char ch);

		/** @brief Remove the next character from the input. */
		void skip() {
			get();
		}

		/** @brief Remove the next character from the input and return it. */
		char get();
		/** @} */


		/** @{ */
		/** @brief The current line number where the scanner is positioned. */
		int lineNo() const noexcept {
			return m_lineNo;
		}

		/** @brief Position of the current line in the input. */
		int linePos() const noexcept {
			return m_linePos;
		}

		/** @brief The position of the scanner in the input stream. */
		int pos() const noexcept {
			return m_pos;
		}
		/** @} */

	private:
		/** @{ */
		[[noreturn]] void badEof() const;
		/** @} */

		std::istream* m_input = nullptr;
		
		int m_linePos = 0;
		int m_lineNo = 0;
		int m_pos = 0;
	};
}

#endif