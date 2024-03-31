#ifndef PARSEC_PARS_LEXER_HEADER
#define PARSEC_PARS_LEXER_HEADER

#include "../core/TextScanner.hpp"
#include "Token.hpp"

#include <gsl/util>

#include <string_view>
#include <optional>
#include <string>
#include <istream>

namespace parsec::pars {
	
	/**
	 * @brief Breaks input text into a sequence of Token%s.
	*/
	class Lexer {
	public:

		Lexer() = default;
		
		explicit Lexer(std::istream& input)
			: m_scanner(input)
		{ }



		/** @{ */
		Lexer(const Lexer&) = delete;
		Lexer& operator=(const Lexer&) = delete;

		Lexer(Lexer&&) = default;
		Lexer& operator=(Lexer&&) = default;
		/** @} */



		/** @{ */
		/**
		 * @brief Perform the input analysis and extract the next token.
		*/
		Token lex() {
			if(m_tok) {
				return *std::exchange(m_tok, {});
			}
			return nextToken();
		}



		/**
		 * @brief Check the next token without extracting it.
		*/
		const Token& peek() {
			if(!m_tok) {
				m_tok = nextToken();
			}
			return *m_tok;
		}



		/**
		 * @brief Check whether the end of input has been reached.
		*/
		bool isEof() {
			return peek().isEof();
		}



		/**
		 * @brief Location of the lexer in the input stream.
		*/
		SourceLoc loc() const noexcept {
			const auto endCol = m_scanner.pos() - m_scanner.line().pos;
			const auto startCol = m_startPos - m_scanner.line().pos;

			return SourceLoc(m_scanner.line(),
				IndexRange(startCol, endCol)
			);
		}
		/** @} */



		/** @{ */
		/**
		 * @brief Remove a token from the input if it is of the specified type.
		 * @returns @c true if a skip has taken place, @c false otherwise.
		*/
		bool skipIf(TokenKinds kind) {
			if(peek().kind() == kind) {
				skip();
				return true;
			}
			return false;
		}



		/**
		 * @brief Remove a token from the input if its text matches specified text.
		 * @returns @c true if a skip has taken place, @c false otherwise.
		*/
		bool skipIf(std::string_view text) {
			if(peek().text() == text) {
				skip();
				return true;
			}
			return false;
		}



		/**
		 * @brief Remove the next token from the input.
		*/
		void skip() {
			lex();
		}
		/** @} */



	private:
		void skipWhitespace();
		void resetParse();

		bool isPatternStart() const;
		bool isIdentStart() const;
		bool isIdent() const;

		TokenKinds parseIdent();
		TokenKinds parsePattern();
		TokenKinds parseOperator();
		TokenKinds parseToken();

		Token nextToken();


		gsl::index m_startPos = {};
		std::optional<Token> m_tok;
		std::string m_buf;

		TextScanner m_scanner;
	};

}

#endif