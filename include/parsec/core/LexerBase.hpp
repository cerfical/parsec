#ifndef PARSEC_LEXER_BASE_HEADER
#define PARSEC_LEXER_BASE_HEADER

#include "TextScanner.hpp"
#include "ParseError.hpp"

#include <string_view>
#include <optional>
#include <string>
#include <istream>

namespace parsec {
	/**
	 * @brief Base for @ref Lexer "lexer"-like types.
	 */
	template <typename Token, typename TokenKinds>
	class LexerBase {
	public:

		LexerBase() = default;

		explicit LexerBase(std::istream& input) noexcept
			: m_scanner(input)
		{ }


		/** @{ */
		LexerBase(LexerBase&&) = default;
		LexerBase& operator=(LexerBase&&) = default;
		/** @} */

		/** @{ */
		LexerBase(const LexerBase&) = delete;
		LexerBase& operator=(const LexerBase&) = delete;
		/** @} */


		/** @{ */
		/** @brief Performs input analysis and extracts the next token. */
		Token lex() {
			if(m_tok) {
				return *std::exchange(m_tok, {});
			}
			return parseToken();
		}

		/** @brief Check the next token without removing it from the input. */
		const Token& peek() const {
			if(!m_tok) {
				m_tok = parseToken();
			}
			return *m_tok;
		}


		/** @brief Check whether the end of file has been reached. */
		bool isEof() const {
			return peek().isEof();
		}
		
		/** @brief Current location of the lexer in the input stream. */
		SourceLoc loc() const noexcept {
			const auto endCol = m_scanner.pos() - m_scanner.line().pos;
			const auto startCol = m_startPos - m_scanner.line().pos;

			return SourceLoc(m_scanner.line(),
				IndexRange(startCol, endCol)
			);
		}
		/** @} */


		/** @{ */
		/** @brief Conditionally removes the next token from the input, depending on its type. */
		bool skipIf(TokenKinds kind) {
			if(peek().kind() == kind) {
				skip();
				return true;
			}
			return false;
		}

		/** @brief Conditionally removes the next token from the input, depending on its textual representation. */
		bool skipIf(std::string_view text) {
			if(peek().text() == text) {
				skip();
				return true;
			}
			return false;
		}

		/** @brief Unconditionally removes the next token from the input. */
		void skip() {
			lex();
		}
		/** @} */


	protected:
		~LexerBase() = default;
		
		
		/** @brief Called when a new token needs to be parsed. */
		virtual TokenKinds onTokenParse() const = 0;

		
		/** @{ */
		/** @brief Generates an error linked to the current lexer position. */
		[[noreturn]] void error() const {
			throw ParseError("malformed token", loc());
		}

		/** @brief Reset the token parse state. */
		void reset() const {
			m_startPos = m_scanner.pos();
			m_buf.clear();
		}


		/** @brief Contributes a new character to the token text. */
		void consume(char ch) const {
			m_buf += ch;
		}

		/** @brief Contributes a character sequence to the token text. */
		void consume(std::string_view str) const {
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