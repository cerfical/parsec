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

		bool isEof() const {
			return peek().isEof();
		}
		/** @} */


		/** @{ */
		bool skipIf(TokenKinds kind) {
			if(peek().kind() == kind) {
				skip();
				return true;
			}
			return false;
		}

		bool skipIf(std::string_view text) {
			if(peek().text() == text) {
				skip();
				return true;
			}
			return false;
		}

		void skip() {
			lex();
		}
		/** @} */


		/** @{ */
		Token expect(TokenKinds kind) {
			if(!peek().is(kind)) {
				invalidToken(describeToken(kind));
			}
			return lex();
		}

		Token expect(std::string_view text) {
			if(peek().text() != text) {
				invalidToken(text);
			}
			return lex();
		}
		/** @} */


	private:
		/** @{ */
		[[noreturn]] void invalidToken(std::string_view expected) const;
		[[noreturn]] void unexpectedEol() const;
		[[noreturn]] void unexpectedChar() const;
		/** @} */


		/** @{ */
		bool isStringLiteral() const;
		bool isIdent() const;
		/** @} */


		/** @{ */
		void skipWhitespace() const;

		TokenKinds parseIdent() const;
		TokenKinds parseStringLiteral() const;
		TokenKinds parseOperator() const;
		
		Token& parseToken() const;
		/** @} */
		
		
		mutable TextScanner m_scanner;

		mutable std::optional<Token> m_tok;
		mutable std::string m_buf;
	};
}

#endif