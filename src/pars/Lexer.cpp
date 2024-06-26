#include "pars/Lexer.hpp"

#include "core/ParseError.hpp"
#include "regex/Parser.hpp"
#include "util/char_util.hpp"

namespace parsec::pars {
	TokenKinds Lexer::parseToken() {
		skipWhitespace();
		resetParse();

		if(m_input.isEof()) {
			return TokenKinds::Eof;
		}

		if(isIdentStart()) {
			return parseIdent();
		}

		if(isStringLiteralStart()) {
			return parseStringLiteral();
		}
		return parseOperator();
	}


	void Lexer::skipWhitespace() {
		while(true) {
			// nothing to skip, the input is empty
			if(m_input.isEof()) {
				break;
			}

			if(char_util::isSpace(m_input.peek())) { // space characters
				m_input.skip();
			} else if(m_input.skipIf("//")) { // single-line comments
				while(!m_input.isEof() && m_input.get() != '\n') {
					// skip until the end of line or file
				}
			} else if(m_input.skipIf("/*")) { // multi-line comments
				while(!m_input.skipIf("*/")) {
					// skip all characters until the end of comment token is encountered
					m_input.skip();
				}
			} else {
				break;
			}
		}
	}


	void Lexer::resetParse() {
		m_tokenStart = m_input.pos().offset;
		m_tokenText.clear();
	}


	TokenKinds Lexer::parseStringLiteral() {
		const auto delim = m_input.get();
		while(true) {
			if(m_input.peek() == '\n') {
				throw ParseError::misplacedChar(m_input.pos(), '\n');
			}

			if(m_input.skipIf(delim)) {
				break;
			}

			// automatically escape regex metachars in raw string literals
			const auto ch = m_input.get();
			if(delim == '\'' && regex::Parser::isMetaChar(ch)) {
				m_tokenText += '\\';
			}
			m_tokenText += ch;

			/**
			 * Provide only minimal handling of escape sequences:
			 *  1. Allows for pattern delimiter (") to be included in the pattern itself
			 *  2. Independence from specific escape sequences supported by regex patterns
			*/
			if(delim == '"' && ch == '\\') {
				m_tokenText += m_input.get();
			}
		}
		return TokenKinds::PatternString;
	}


	bool Lexer::isStringLiteralStart() const {
		if(m_input.isEof()) {
			return false;
		}

		const auto ch = m_input.peek();
		if(ch == '"' || ch == '\'') {
			return true;
		}
		return false;
	}


	TokenKinds Lexer::parseIdent() {
		while(isIdentStart() || char_util::isDigit(m_input.peek())) {
			m_tokenText += m_input.get();
		}
		return TokenKinds::Ident;
	}


	bool Lexer::isIdentStart() const {
		const auto ch = m_input.peek();
		return !m_input.isEof()
			&& (char_util::isAlpha(ch) || ch == '-' || ch == '_');
	}


	TokenKinds Lexer::parseOperator() {
		TokenKinds kind = {};
		switch(m_input.peek()) {
			case '{': kind = TokenKinds::LeftBrace; break;
			case '}': kind = TokenKinds::RightBrace; break;
			case '(': kind = TokenKinds::LeftParen; break;
			case ')': kind = TokenKinds::RightParen; break;
			case ';': kind = TokenKinds::Semicolon; break;
			case '=': kind = TokenKinds::Equals; break;
			case '|': kind = TokenKinds::Pipe; break;
			case '*': kind = TokenKinds::Star; break;
			case '+': kind = TokenKinds::Plus; break;
			case '?': kind = TokenKinds::QuestionMark; break;
			default: {
				throw ParseError::invalidChar(m_input.pos(), m_input.peek());
			}
		}
		m_tokenText += m_input.get();
		return kind;
	}
}