#include "pars/Lexer.hpp"
#include "pars/ParseError.hpp"

#include "regex/Parser.hpp"
#include "core/EofError.hpp"
#include "utils/char_utils.hpp"

#include <format>

namespace parsec::pars {
	void Lexer::unexpectedEolError() {
		throw ParseError("unexpected end of line", loc());
	}

	void Lexer::unexpectedCharError() {
		throw ParseError(std::format("unexpected '{}'",
			char_utils::escape(m_scanner.peek())
		), loc());
	}


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
				unexpectedEolError();
			}

			if(m_scanner.skipIf(delim)) {
				break;
			}
			
			// automatically escape regex metachars in raw patterns
			const auto ch = m_scanner.get();
			if(delim == '\'' && regex::Parser::isMetachar(ch)) {
				m_buf += '\\';
			}
			m_buf += ch;

			// provide only minimal handling of escape sequences:
			//  1. to allow pattern delimiters to be included in the pattern itself
			//  2. to be independent of specific implemented escape sequences
			if(ch == '\\') {
				m_buf += m_scanner.get();
			}
		}
		return TokenKinds::Pattern;
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
				unexpectedCharError();
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
		try {
			const auto kind = parseToken();
			return Token(
				m_buf, kind, loc()
			);
		} catch(const EofError& e) {
			throw ParseError("unexpected end of file", loc());
		}
	}
}