#pragma once

#include "../utils/util_types.hpp"
#include "SourceLoc.hpp"

#include <string_view>
#include <istream>
#include <string>

namespace parsec {

	/**
	 * @brief Facilitates textual data analysis using standard stream classes.
	*/
	class TextScanner : private NonCopyable {
	public:

		TextScanner() = default;

		explicit TextScanner(std::istream& input)
			: m_input(&input) {}



		/** @{ */
		/**
		 * @brief Read and remove from the stream the next input character.
		*/
		char get();

		

		/**
		 * @brief Read a character at the specified offset from the current position without removing it from the stream.
		*/
		char peek(int i = 0) const;



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
		 * @brief Check whether the end of input has been reached.
		*/
		bool isEof() const {
			return m_labuf.empty() && checkForEof();
		}



		/**
		 * @brief Information about the position of the scanner in the input stream.
		*/
		SourceLoc loc() const {
			return SourceLoc(m_inputPos - m_linePos, m_lineNo, m_linePos);
		}
		/** @} */



	private:
		bool checkForEof() const;
		void updateLoc(char ch);

		bool fillLookaheadBuffer(std::size_t size) const;

		mutable std::string m_labuf;
		std::istream* m_input = {};

		int m_inputPos = 0;
		int m_linePos = 0;
		int m_lineNo = 0;
	};

}
