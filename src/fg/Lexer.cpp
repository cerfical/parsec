#include "fg/Lexer.hpp"

#include "utils/ParseError.hpp"
#include "utils/chars.hpp"

#include <sstream>

namespace parsec::fg {
	void Lexer::parseError(gsl::czstring msg) {
		// construct a location that points to the offending symbol
		const auto loc = SourceLoc(
			m_input.pos() - m_input.linePos(), 1,
			m_input.lineNo(), m_input.linePos()
		);
		throw ParseError(msg, loc);
	}

	void Lexer::badCharError() {
		const auto msg = (std::ostringstream()
			<< "unexpected \'" << m_input.peek() << '\''
		).str();
		parseError(msg.c_str());
	}
	

	void Lexer::skipWhitespace() {
		while(!m_input.eof() && isSpace(m_input.peek())) {
			m_input.skip();
		}
	}


	bool Lexer::identStart() {
		return !m_input.eof() && (isAlnum(m_input.peek())
				|| m_input.peek() == '-'
				|| m_input.peek() == '_'
		);
	}
	
	Token::Kinds Lexer::parseIdent() {
		while(identStart()) {
			m_buf += m_input.get();
		}
		return Token::Ident;
	}


	bool Lexer::stringLiteralStart() {
		return !m_input.eof() && (
			m_input.peek() == '\'' || m_input.peek() == '\"'
		);
	}

	Token::Kinds Lexer::parseStringLiteral() {
		const auto delim = m_input.get();
		while(true) {
			if(m_input.peek() == '\n') {
				parseError("unexpected end of line");
			}
			if(m_input.skipIf(delim)) {
				break;
			}
			
			const auto ch = m_input.get();
			m_buf += ch;

			// ignore delimiters that are escaped with backslashes
			if(ch == '\\' && m_input.peek() == delim) {
				m_buf += m_input.get();
			}
		}
		return Token::StringLiteral;
	}


	Token::Kinds Lexer::parseOperator() {
		auto tokKind = Token::Eof;
		switch(m_input.peek()) {
			case '{': tokKind = Token::OpenBrace; break;
			case '}': tokKind = Token::CloseBrace; break;
			case '(': tokKind = Token::OpenParen; break;
			case ')': tokKind = Token::CloseParen; break;
			case ';': tokKind = Token::Semicolon; break;
			case '=': tokKind = Token::Equals; break;
			case '|': tokKind = Token::Pipe; break;
			default: badCharError();
		}
		m_buf += m_input.get();
		return tokKind;
	}
	

	Token& Lexer::parseToken() {
		// do nothing if the token is already parsed
		if(m_tok) {
			return *m_tok;
		}

		// remove all whitespace from the input before parsing the token
		skipWhitespace();

		const auto startPos = m_input.pos();
		m_buf.clear(); // clear the text buffer

		// parse the token according to its type
		auto tokKind = Token::Eof;
		if(!m_input.eof()) {
			if(identStart()) {
				tokKind = parseIdent();
			} else if(stringLiteralStart()) {
				tokKind = parseStringLiteral();
			} else {
				tokKind = parseOperator();
			}
		}

		// calculate proper values for the starting column and the number of columns of the token
		const auto colCount = tokKind != Token::Eof ? m_input.pos() - startPos : 1;
		const auto startCol = startPos - m_input.linePos();

		const auto loc = SourceLoc(startCol, colCount, m_input.lineNo(), m_input.linePos());
		m_tok.emplace(tokKind, m_buf, loc);
		return *m_tok;
	}


	bool Lexer::skipIf(Token::Kinds kind) {
		if(peek().kind() == kind) {
			skip();
			return true;
		}
		return false;
	}

	Token Lexer::lex() {
		auto tok = std::move(parseToken());
		m_tok.reset();
		return tok;
	}
}