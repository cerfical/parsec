#ifndef PARSEC_FG_LEXER_HEADER
#define PARSEC_FG_LEXER_HEADER

#include "../utils/TextScanner.hpp"
#include "Token.hpp"

#include <optional>

namespace parsec::fg {
	class Lexer {
	public:
		/** @{ */
		explicit Lexer(std::istream& input = std::cin) noexcept
			: m_scanner(input)
		{ }
		/** @} */


		/** @{ */
		Lexer(Lexer&&) = default;
		Lexer& operator=(Lexer&&) = default;
		/** @} */

		/** @{ */
		Lexer(const Lexer&) = delete;
		Lexer& operator=(const Lexer&) = delete;
		/** @} */


		/** @{ */
		Token lex() {
			auto tok = std::move(parseToken());
			m_tok.reset();
			return tok;
		}

		const Token& peek() const {
			return parseToken();
		}

		bool eof() const {
			return peek().eof();
		}
		/** @} */


		/** @{ */
		bool skipIf(TokenKinds tok) {
			if(peek().kind() == tok) {
				skip();
				return true;
			}
			return false;
		}

		bool skipIf(const std::string& text) {
			if(peek().text() == text) {
				skip();
				return true;
			}
			return false;
		}

		void skip() {
			lex();
		}


		Token expect(TokenKinds tok) {
			if(!peek().is(tok)) {
				invalidToken(describeToken(tok));
			}
			return lex();
		}

		Token expect(const std::string& tok) {
			if(peek().text() != tok) {
				invalidToken(tok);
			}
			return lex();
		}
		/** @} */


	private:
		/** @{ */
		[[noreturn]] void invalidToken(const std::string& expected) const;
		[[noreturn]] void unexpectedEol() const;
		[[noreturn]] void unexpectedChar() const;
		/** @} */


		/** @{ */
		bool stringLiteralStart() const;
		bool identStart() const;
		/** @} */


		/** @{ */
		void skipWhitespace() const;

		TokenKinds parseIdent() const;
		TokenKinds parseStringLiteral() const;
		TokenKinds parseOperator() const;
		
		Token& parseToken() const;
		/** @} */
		
		
		/** @{ */
		mutable TextScanner m_scanner;
		/** @} */


		/** @{ */
		mutable std::optional<Token> m_tok;
		mutable std::string m_buf;
		/** @} */
	};
}

#endif