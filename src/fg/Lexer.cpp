#include "fg/Lexer.hpp"

#include "utils/Error.hpp"
#include "utils/chars.hpp"

#include <format>

namespace parsec::fg {
	void Lexer::invalidToken(std::string_view expected) const {
		throw Error(
			std::format(
				"expected {}, but found {}",
				expected,
				describeToken(
					peek().kind()
				)
			),
			peek().loc()
		);
	}

	void Lexer::unexpectedEol() const {
		throw Error(
			"unexpected end of line",
			m_scanner.loc()
		);
	}

	void Lexer::unexpectedChar() const {
		throw Error("unexpected '"
				+ escapeChar(m_scanner.peek())
				+ "'",
			m_scanner.loc()
		);
	}
	

	bool Lexer::isStringLiteral() const {
		return !m_scanner.isEof()
			&& (m_scanner.peek() == '\''
				|| m_scanner.peek() == '\"'
			);
	}

	bool Lexer::isIdent() const {
		return !m_scanner.isEof()
			&& (isAlnum(m_scanner.peek())
				|| m_scanner.peek() == '-'
				|| m_scanner.peek() == '_'
			);
	}


	void Lexer::skipWhitespace() const {
		while(true) {
			// nothing to skip, the input is empty
			if(m_scanner.isEof()) {
				break;
			}

			if(isSpace(m_scanner.peek())) { // space characters
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
	
	TokenKinds Lexer::parseIdent() const {
		while(isIdent()) {
			m_buf += m_scanner.get();
		}
		return TokenKinds::Ident;
	}

	TokenKinds Lexer::parseStringLiteral() const {
		const auto delim = m_scanner.get();
		while(true) {
			if(m_scanner.peek() == '\n') {
				unexpectedEol();
			}
			if(m_scanner.skipIf(delim)) {
				break;
			}
			
			const auto ch = m_scanner.get();
			m_buf += ch;

			// ignore delimiters that are escaped with backslashes
			if(ch == '\\' && m_scanner.peek() == delim) {
				m_buf += m_scanner.get();
			}
		}
		return TokenKinds::StringLiteral;
	}

	TokenKinds Lexer::parseOperator() const {
		auto tokKind = TokenKinds::Eof;
		switch(m_scanner.peek()) {
			case '{': tokKind = TokenKinds::OpenBrace; break;
			case '}': tokKind = TokenKinds::CloseBrace; break;
			case '(': tokKind = TokenKinds::OpenParen; break;
			case ')': tokKind = TokenKinds::CloseParen; break;
			case ';': tokKind = TokenKinds::Semicolon; break;
			case '=': tokKind = TokenKinds::Equals; break;
			case '|': tokKind = TokenKinds::Pipe; break;
			default: unexpectedChar();
		}
		m_buf += m_scanner.get();
		return tokKind;
	}
	
	Token& Lexer::parseToken() const {
		// do nothing if the token is already parsed
		if(m_tok) {
			return *m_tok;
		}

		// remove all whitespace from the input before parsing the token
		skipWhitespace();

		const auto startPos = m_scanner.pos();
		m_buf.clear();

		// parse the token according to its type
		auto tokKind = TokenKinds::Eof;
		if(!m_scanner.isEof()) {
			if(isIdent()) {
				tokKind = parseIdent();
			} else if(isStringLiteral()) {
				tokKind = parseStringLiteral();
			} else {
				tokKind = parseOperator();
			}
		}

		// calculate proper values for the starting column and the number of columns of the token
		const auto colCount = std::max(m_scanner.pos() - startPos, 1);
		const auto startCol = startPos - m_scanner.linePos();

		m_tok.emplace(m_buf, tokKind,
			SourceLoc(
				startCol,
				colCount,
				m_scanner.lineNo(),
				m_scanner.linePos()
			)
		);

		return *m_tok;
	}
}