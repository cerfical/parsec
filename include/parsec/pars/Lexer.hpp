#pragma once

#include "../utils/util_types.hpp"
#include "../core/TextScanner.hpp"

#include "Token.hpp"

#include <string_view>
#include <optional>
#include <string>
#include <istream>

namespace parsec::pars {
	
	/**
	 * @brief Breaks input text into a sequence of Token%s.
	*/
	class Lexer : private NonCopyable {
	public:

		Lexer() = default;
		
		explicit Lexer(std::istream& input)
			: m_input(input) {}



		/** @{ */
		/**
		 * @brief Perform the input analysis and extract the next token.
		*/
		Token lex() {
			if(!m_token) {
				m_token = nextToken();
			}
			
			auto tok = std::move(m_token.value());
			m_token.reset();
			return tok;
		}



		/**
		 * @brief Check the next token without extracting it.
		*/
		const Token& peek() {
			if(!m_token) {
				m_token = nextToken();
			}
			return *m_token;
		}



		/**
		 * @brief Check whether the end of input has been reached.
		*/
		bool isEof() {
			return peek().is<TokenKinds::Eof>();
		}



		/**
		 * @brief Location of the lexer in the input stream.
		*/
		SourceLoc loc() const {
			const auto inputLoc = m_input.loc();

			const auto startCol = m_tokenStart - inputLoc.linePos();
			const auto colCount = inputLoc.pos() - m_tokenStart;

			return SourceLoc(
				startCol,
				colCount,
				inputLoc.lineNo(),
				inputLoc.linePos()
			);
		}
		/** @} */



		/** @{ */
		/**
		 * @brief Remove a token from the input if it is of the specified type.
		 * @returns @c true if a skip has taken place, @c false otherwise.
		*/
		bool skipIf(TokenKinds tok) {
			if(peek().kind() == tok) {
				skip();
				return true;
			}
			return false;
		}



		/**
		 * @brief Remove a token from the input if its text matches specified text.
		 * @returns @c true if a skip has taken place, @c false otherwise.
		*/
		bool skipIf(std::string_view tok) {
			if(peek().text() == tok) {
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
		Token nextToken() {
			const auto kind = parseToken();
			return Token(
				m_tokenText, kind, loc()
			);
		}

		void skipWhitespace();
		void resetParse();

		TokenKinds parseStringLiteral();
		bool isStringLiteralStart() const;
		
		TokenKinds parseIdent();
		bool isIdentStart() const;

		TokenKinds parseOperator();
		TokenKinds parseToken();

		int m_tokenStart = {};
		std::optional<Token> m_token;
		std::string m_tokenText;

		TextScanner m_input;
	};

}
