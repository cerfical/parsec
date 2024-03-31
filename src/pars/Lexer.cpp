#include "pars/Lexer.hpp"

#include "errors/InvalidCharError.hpp"
#include "errors/UnexpectedEofError.hpp"

#include "utils/char_utils.hpp"

namespace parsec::pars {
	void Lexer::skipWhitespace() {
		while(true) {
			// nothing to skip, the input is empty
			if(m_scanner.isEof()) {
				break;
			}

			if(char_utils::isSpace(m_scanner.peek())) { // space characters
				m_scanner.skip();
			} else if(m_scanner.skipIf("//")) { // single-line comments
				while(!m_scanner.isEof() && m_scanner.get() != '\n') {
					// skip until the end of line or file
				}
			} else if(m_scanner.skipIf("/*")) { // multi-line comments
				while(!m_scanner.skipIf("*/")) {
					// skip all characters until the end of comment token is encountered
					m_scanner.skip();
				}
			} else {
				break;
			}
		}
	}

	void Lexer::resetParse() {
		m_startPos = m_scanner.pos();
		m_buf.clear();
	}


	bool Lexer::isPatternStart() const {
		if(m_scanner.isEof()) {
			return false;
		}

		const auto ch = m_scanner.peek();
		if(ch == '\'' || ch == '\"') {
			return true;
		}
		return false;
	}

	bool Lexer::isIdentStart() const {
		const auto ch = m_scanner.peek();
		return !m_scanner.isEof()
			&& (char_utils::isAlpha(ch) || ch == '-' || ch == '_');
	}

	bool Lexer::isIdent() const {
		return isIdentStart() || char_utils::isDigit(m_scanner.peek());
	}


	TokenKinds Lexer::parseIdent() {
		while(isIdent()) {
			m_buf += m_scanner.get();
		}
		return TokenKinds::Ident;
	}

	TokenKinds Lexer::parsePattern() {
		const auto delim = m_scanner.get();
		while(true) {
			if(m_scanner.peek() == '\n') {
				throw UnexpectedEofError(loc());
			}

			if(m_scanner.skipIf(delim)) {
				break;
			}
			
			const auto ch = m_scanner.get();
			m_buf += ch;

			/**
			 * Provide only minimal handling of escape sequences:
			 *  1. Allows for pattern delimiter (") to be included in the pattern itself
			 *  2. Independence from specific escape sequences supported by regex patterns
			*/
			if(delim == '"' && ch == '\\') {
				m_buf += m_scanner.get();
			}
		}
		return delim == '"' ? TokenKinds::PatternString : TokenKinds::RawString;
	}

	TokenKinds Lexer::parseOperator() {
		TokenKinds kind = {};
		switch(m_scanner.peek()) {
			case '{': kind = TokenKinds::LeftBrace; break;
			case '}': kind = TokenKinds::RightBrace; break;
			case '(': kind = TokenKinds::LeftParen; break;
			case ')': kind = TokenKinds::RightParen; break;
			case ';': kind = TokenKinds::Semicolon; break;
			case '=': kind = TokenKinds::Equals; break;
			case '|': kind = TokenKinds::Pipe; break;
			case '*': kind = TokenKinds::Star; break;
			case '+': kind = TokenKinds::Plus; break;
			case '?': kind = TokenKinds::Qo; break;
			default: {
				throw InvalidCharError(loc(), m_scanner.peek());
			}
		}
		m_buf += m_scanner.get();
		return kind;
	}

	TokenKinds Lexer::parseToken() {
		skipWhitespace();
		resetParse();

		if(m_scanner.isEof()) {
			return TokenKinds::Eof;
		}
		
		if(isIdentStart()) {
			return parseIdent();
		}
		
		if(isPatternStart()) {
			return parsePattern();
		}
		return parseOperator();
	}


	Token Lexer::nextToken() {
		const auto kind = parseToken();
		return Token(
			m_buf, kind, loc()
		);
	}
}