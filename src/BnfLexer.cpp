#include "BnfLexer.hpp"

#include <gsl/narrow>
#include <sstream>

namespace parsec {
	void BnfLexer::UnexpectedChar() const {
		const auto ch = PeekChar();
		const auto msg = (std::ostringstream()
			<< "unexpected \'" << ch << '\''
		).str();

		throw ParseError(msg, GetInputPos());
	}
	void BnfLexer::UnexpectedEof() const {
		throw ParseError("unexpected end of file", GetInputPos());
	}

	bool BnfLexer::IsInputEnd() const {
		if(input) {
			// if the of end of file was reached, reset the eof bit
			if(input->peek() != std::char_traits<char>::eof()) {
				return false;
			}
			input->clear(input->rdstate() ^ std::ios_base::eofbit);
		}
		return true;
	}
	bool BnfLexer::IsTokenParsed() const noexcept {
		return tokKind != BnfTokenKinds::Eof;
	}

	char BnfLexer::PeekChar() const {
		if(!IsInputEnd()) {
			return gsl::narrow_cast<char>(input->peek());
		}
		UnexpectedEof();
	}
	bool BnfLexer::SkipCharIf(char ch) {
		if(!IsInputEnd()) {
			if(PeekChar() == ch) {
				SkipChar();
				return true;
			}
		}
		return false;
	}	
	void BnfLexer::SkipChar() {
		GetChar();
	}
	char BnfLexer::GetChar() {
		if(!IsInputEnd()) {
			colCount++;
			return gsl::narrow_cast<char>(input->get());
		}
		UnexpectedEof();
	}

	void BnfLexer::SkipWhitespace() {
		while(!IsInputEnd() && CharUtils::IsSpace(PeekChar())) {
			if(GetChar() == '\n') {
				lineStart += std::exchange(colNo, 0) + std::exchange(colCount, 0);
				lineNo++;
			}
		}
	}

	bool BnfLexer::IsIdentStart() const {
		return !IsInputEnd() && (
			CharUtils::IsAlnum(PeekChar())
				|| PeekChar() == '-'
				|| PeekChar() == '_'
		);
	}
	void BnfLexer::ParseIdent() {
		while(IsIdentStart()) {
			tokText += GetChar();
		}
		tokKind = BnfTokenKinds::Ident;
	}

	bool BnfLexer::IsRegexStart() const {
		return !IsInputEnd() && (
			PeekChar() == '\'' || PeekChar() == '\"'
		);
	}
	void BnfLexer::ParseRegex() {
		const auto regexDelim = GetChar();
		while(true) {
			if(PeekChar() == '\n') {
				throw ParseError("unexpected end of line", GetInputPos());
			}
			if(SkipCharIf(regexDelim)) {
				break;
			}
			
			const auto ch = GetChar();
			tokText += ch;

			if(ch == '\\' && PeekChar() == regexDelim) {
				tokText += GetChar();
			}
		}
		tokKind = BnfTokenKinds::Regex;
	}

	void BnfLexer::ParseOperator() {
		switch(PeekChar()) {
			case '{': tokKind = BnfTokenKinds::OpenBrace; break;
			case '}': tokKind = BnfTokenKinds::CloseBrace; break;
			case ';': tokKind = BnfTokenKinds::Semicolon; break;
			case '=': tokKind = BnfTokenKinds::Equals; break;
			case '|': tokKind = BnfTokenKinds::Pipe; break;
			default: {
				UnexpectedChar();
			}
		}
		tokText += GetChar();
	}
	void BnfLexer::ParseToken() {
		// remove all whitespace from the input before parsing the token
		SkipWhitespace();
		colNo += std::exchange(colCount, 0);
		tokText.clear(); // clear the text buffer

		// parse the token according to its type
		if(!IsInputEnd()) {
			if(IsIdentStart()) {
				ParseIdent();
			} else if(IsRegexStart()) {
				ParseRegex();
			} else {
				ParseOperator();
			}
		} else {
			tokKind = BnfTokenKinds::Eof;
		}
	}

	SourceLocation BnfLexer::GetInputPos() const noexcept {
		if(IsTokenParsed()) {
			return SourceLocation(
				lineStart, colNo, colCount, lineNo
			);
		}
		return SourceLocation(lineStart, colNo, 1, lineNo);
	}

	BnfTokenKinds BnfLexer::Peek() {
		if(!IsTokenParsed()) {
			ParseToken();
		}
		return tokKind;		
	}
	BnfToken BnfLexer::Lex() {
		if(!IsTokenParsed()) {
			ParseToken();
		}
		
		return BnfToken(
			tokText, std::exchange(tokKind, BnfTokenKinds::Eof), GetInputPos()
		);
	}
}