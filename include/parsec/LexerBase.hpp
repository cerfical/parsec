#ifndef PARSEC_LEXER_BASE_HEADER
#define PARSEC_LEXER_BASE_HEADER

#include "TextScanner.hpp"
#include "Error.hpp"

#include <string_view>
#include <optional>
#include <string>
#include <istream>

namespace parsec {
	template <typename Token, typename TokenKinds>
	class LexerBase {
	public:
		/** @{ */
		explicit LexerBase(std::istream& input = std::cin) noexcept
			: m_scanner(input)
		{ }
		/** @} */


		/** @{ */
		LexerBase(LexerBase&&) = default;
		LexerBase& operator=(LexerBase&&) = default;
		/** @} */

		/** @{ */
		LexerBase(const LexerBase&) = delete;
		LexerBase& operator=(const LexerBase&) = delete;
		/** @} */


		/** @{ */
		Token lex() {
			if(m_tok) {
				return *std::exchange(m_tok, {});
			}
			return parseToken();
		}

		const Token& peek() const {
			if(!m_tok) {
				m_tok = parseToken();
			}
			return *m_tok;
		}

		bool isEof() const {
			return peek().isEof();
		}


		SourceLoc loc() const noexcept {
			// calculate proper values for the number of columns and the starting column
			const auto colCount = std::max(m_scanner.pos() - m_startPos, 1);
			const auto startCol = m_startPos - m_scanner.linePos();

			return SourceLoc(
				startCol,
				colCount,
				m_scanner.lineNo(),
				m_scanner.linePos()
			);
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


	protected:
		/** @{ */
		~LexerBase() = default;
		/** @} */
		
		
		/** @{ */
		virtual typename TokenKinds onTokenParse() const = 0;
		/** @} */

		
		/** @{ */
		[[noreturn]] void error() const {
			throw Error("malformed token", loc());
		}

		void reset() const {
			m_startPos = m_scanner.pos();
			m_buf.clear();
		}


		void consume(char ch) const {
			m_buf += ch;
		}

		void consume(const std::string& str) const {
			m_buf += str;
		}
	
		
		TextScanner* scanner() const {
			return &m_scanner;
		}
		/** @} */


	private:
		Token parseToken() const {
			const auto kind = onTokenParse();
			return Token(
				m_buf, kind, loc()
			);
		}


		mutable TextScanner m_scanner;

		mutable std::optional<Token> m_tok;
		mutable std::string m_buf;
		mutable int m_startPos = 0;
	};
}

#endif