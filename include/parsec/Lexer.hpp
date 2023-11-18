#ifndef PARSEC_LEXER_HEADER
#define PARSEC_LEXER_HEADER

#include "TextScanner.hpp"
#include "Token.hpp"

#include <optional>

namespace parsec {
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
				unexpectedToken(Token::describe(kind));
			}
			return lex();
		}
		
		void match(TokenKinds kind) {
			if(!peek().is(kind)) {
				unexpectedToken(Token::describe(kind));
			}
			skip();
		}
		/** @} */


	private:
		/** @{ */
		[[noreturn]] void unexpectedToken(std::string_view expected) const;
		[[noreturn]] void unexpectedEol() const;
		[[noreturn]] void unexpectedChar() const;
		/** @} */


		/** @{ */
		bool isPatternStart() const;
		bool isIdentStart() const;
		bool isWordStart() const;
		/** @} */


		/** @{ */
		void skipWhitespace() const;

		TokenKinds parseIdent() const;
		TokenKinds parsePattern() const;
		TokenKinds parseOperator() const;
		
		Token& parseToken() const;
		/** @} */
		
		
		mutable TextScanner m_scanner;

		mutable std::optional<Token> m_tok;
		mutable std::string m_buf;
	};
}

#endif