#include "Lexer.hpp"
#include "Error.hpp"

#include "regex/Parser.hpp"

#include "utils/strings.hpp"
#include "utils/chars.hpp"

#include <format>

namespace parsec {
	void Lexer::unexpectedToken(std::string_view expected) const {
		throw Error(
			std::format("expected {}, but found {}",
				expected,
				Token::describe(
					peek().kind()
				)
			),
			peek().loc()
		);
	}

	void Lexer::unexpectedEol() const {
		throw Error("unexpected end of line",
			m_scanner.loc()
		);
	}

	void Lexer::unexpectedChar() const {
		throw Error(std::format("unexpected '{}'",
				escapeChar(m_scanner.peek())
			),
			m_scanner.loc()
		);
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
		// identifiers must start with a letter
		return !m_scanner.isEof()
			&& (isAlpha(m_scanner.peek()));
	}

	bool Lexer::isWordStart() const {
		if(m_scanner.isEof()) {
			return false;
		}

		const auto ch = m_scanner.peek();
		const auto ch1 = m_scanner.peek(1);
		
		// words inside the identifier are defined by either the presence of some delimeter
		if((ch == '-' || ch == '_') && isAlnum(ch1)) {
			m_scanner.skip();
			return true;
		}

		// or a letter
		if(isAlpha(ch)) {
			return true;
		}

		return false;
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
		std::string upper, lower;
		while(isWordStart()) {
			// read uppercase part of a word
			while(!m_scanner.isEof()) {
				const auto ch = m_scanner.peek();
				if(!isUpper(ch) && !isDigit(ch)) {
					break;
				}
				upper += m_scanner.get();
			}

			// read lowercase part of a word
			while(!m_scanner.isEof()) {
				const auto ch = m_scanner.peek();
				if(!isLower(ch) && !isDigit(ch)) {
					break;
				}
				lower += m_scanner.get();
			}

			// combine the upper and lower case parts to create a valid capitalized word
			if(lower.empty()) {
				m_buf += Strings::capitalize(upper);
			} else if(upper.empty()) {
				m_buf += Strings::capitalize(lower);
			} else {
				m_buf += upper + lower;
			}

			upper.clear();
			lower.clear();
		}
		return TokenKinds::Ident;
	}

	TokenKinds Lexer::parsePattern() const {
		const auto delim = m_scanner.get();
		while(true) {
			if(m_scanner.peek() == '\n') {
				unexpectedEol();
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

			// handle escaped characters
			if(ch == '\\') {
				m_buf += m_scanner.get();
			}
		}
		return TokenKinds::Pattern;
	}

	TokenKinds Lexer::parseOperator() const {
		TokenKinds tokKind = {};
		switch(m_scanner.peek()) {
			case '{': tokKind = TokenKinds::OpenBrace; break;
			case '}': tokKind = TokenKinds::CloseBrace; break;
			case '(': tokKind = TokenKinds::OpenParen; break;
			case ')': tokKind = TokenKinds::CloseParen; break;
			case ';': tokKind = TokenKinds::Semicolon; break;
			case '=': tokKind = TokenKinds::Equals; break;
			case '|': tokKind = TokenKinds::Pipe; break;
			case '*': tokKind = TokenKinds::Star; break;
			case '+': tokKind = TokenKinds::Plus; break;
			case '?': tokKind = TokenKinds::Qo; break;
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
			if(isIdentStart()) {
				tokKind = parseIdent();
			} else if(isPatternStart()) {
				tokKind = parsePattern();
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