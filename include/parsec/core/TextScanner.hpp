#ifndef PARSEC_CORE_TEXT_SCANNER_HEADER
#define PARSEC_CORE_TEXT_SCANNER_HEADER

#include "SourceLoc.hpp"
#include <gsl/util>

#include <string_view>
#include <istream>
#include <string>

namespace parsec {

	/**
	 * @brief Facilitates textual data analysis using standard stream classes.
	*/
	class TextScanner {
	public:

		TextScanner() = default;

		TextScanner(std::istream& input)
			: m_input(&input)
		{ }



		/** @{ */
		TextScanner(const TextScanner&) = delete;
		TextScanner& operator=(const TextScanner&) = delete;

		TextScanner(TextScanner&&) = default;
		TextScanner& operator=(TextScanner&&) = default;
		/** @} */



		/** @{ */
		/**
		 * @brief Read and remove from the stream the next input character.
		*/
		char get();

		

		/**
		 * @brief Read a character at the specified offset from the current position without removing it from the stream.
		*/
		char peek(gsl::index i = 0) const;


		
		/**
		 * @brief Check whether the end of input has been reached.
		*/
		bool isEof() const {
			return m_labuf.empty() && checkForEof();
		}
		/** @} */



		/** @{ */
		/**
		 * @brief Remove a character from the input if it matches specified character.
		 * @returns @c true if a skip has taken place, @c false otherwise.
		*/
		bool skipIf(char ch);


		
		/**
		 * @brief Remove a character sequence from the input if it matches specified text.
		 * @returns @c true if a skip has taken place, @c false otherwise.
		*/
		bool skipIf(std::string_view text);



		/**
		 * @brief Remove the next character from the input.
		*/
		void skip() {
			get();
		}
		/** @} */



		/** @{ */
		/**
		 * @brief Position of the scanner in the input stream.
		*/
		int pos() const noexcept {
			return m_inputPos;
		}


		
		/**
		 * @brief Compact representation of the scanner position.
		*/
		SourceLoc loc() const noexcept {
			return SourceLoc(m_inputPos - m_linePos, m_lineNo, m_linePos);
		}
		/** @} */



	private:
		[[noreturn]] void unexpectedEof() const;


		bool checkForEof() const;
		void updateLoc(char ch);

		bool fillBuf(std::size_t size) const;


		mutable std::string m_labuf;
		std::istream* m_input = {};

		int m_inputPos = 0;
		int m_linePos = 0;
		int m_lineNo = 0;
	};

}

#endif