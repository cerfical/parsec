#include "Lexer.hpp"
#include <sstream>

namespace parsec {
	SourceLocation Lexer::MakeTokenLocation() const noexcept {
		const auto loc = SourceLocation({
			.chars = CharRange({ .startPos = pos, .size = tokText.size() }),
			.lineNo = lineNo,
			.colNo = colNo
		});
		return loc;
	}
	
	SourceLocation Lexer::GetInputPos() const noexcept {
		const auto loc = SourceLocation({
			.chars = CharRange({ .startPos = pos, .size = 1 }),
			.lineNo = lineNo,
			.colNo = colNo
		});
		return loc;
	}

	bool Lexer::IsInputEnd() const {
		return input->eof() || input->peek() == std::char_traits<char>::eof();
	}

	char Lexer::PeekChar() const {
		return static_cast<char>(input->peek());
	}
	void Lexer::SkipChar() {
		input->ignore();
	}
	char Lexer::GetChar() {
		pos++;
		return static_cast<char>(input->get());
	}

	void Lexer::SkipWhitespace() {
		while(!IsInputEnd() && CharUtils::IsSpace(PeekChar())) {
			if(PeekChar() == '\n') {
				colNo = 0;
				lineNo++;
			}
			SkipChar();
		}
	}

	bool Lexer::IsIdentStart() const {
		return !IsInputEnd() && (
			CharUtils::IsAlnum(PeekChar())
				|| PeekChar() == '-'
				|| PeekChar() == '_'
		);
	}

	void Lexer::ParseIdent() {
		while(IsIdentStart()) {
			tokText += GetChar();
		}
		tokKind = TokenKinds::Ident;
	}

	bool Lexer::IsRegexStart() const {
		return !IsInputEnd() && (
			PeekChar() == '\'' || PeekChar() == '\"'
		);
	}

	void Lexer::ParseRegex() {
		const auto regexDelim = GetChar();
		while(!IsInputEnd() && PeekChar() != regexDelim) {
			if(const auto loc = GetInputPos(); PeekChar() == '\n') {
				throw ParseError("unexpected end of line", loc);
			}
			tokText += GetChar();
		}
		if(const auto loc = GetInputPos(); IsInputEnd()) {
			throw ParseError("unexpected end of file", loc);
		}
		SkipChar();
		tokKind = TokenKinds::Regex;
	}

	void Lexer::ParseOperator() {
		if(!IsInputEnd()) {
			switch(PeekChar()) {
				case '{': tokKind = TokenKinds::LeftBrace; break;
				case '}': tokKind = TokenKinds::RightBrace; break;
				case ';': tokKind = TokenKinds::Semicolon; break;
				case '=': tokKind = TokenKinds::Equals; break;
				case '|': tokKind = TokenKinds::Pipe; break;
				default: {
					const auto loc = GetInputPos();
					const auto msg = (std::ostringstream()
						<< "unexpected \'" << PeekChar() << '\''
					).str();
					throw ParseError(msg, loc);
				}
			}
			tokText += GetChar();
		}
	}

	Token Lexer::Lex() {
		tokText.clear(); // clear text buffer
		colNo++; // move to the next column
		
		// skip all whitespace before parsing the token
		SkipWhitespace();
		if(!IsInputEnd()) {
			if(IsIdentStart()) {
				ParseIdent();
			} else if(IsRegexStart()) {
				ParseRegex();
			} else {
				ParseOperator();
			}
		} else {
			tokKind = TokenKinds::Eof;
		}

		// create token out of parsed input
		const auto tokLoc = MakeTokenLocation();
		return Token(tokText, tokKind, tokLoc);
	}
}