#include "BnfLexer.hpp"

#include <gsl/narrow>
#include <sstream>

namespace parsec {
	bool BnfLexer::IsInputEnd() const {
		// in case of end of file, reset the eof bit and return true
		if(input->peek() == std::char_traits<char>::eof()) {
			input->clear(input->rdstate() ^ std::ios_base::eofbit);
			return true;
		}
		return false;
	}
	bool BnfLexer::IsTokenParsed() const noexcept {
		return tokKind != BnfTokenKinds::Eof;
	}

	char BnfLexer::PeekChar() const {
		return gsl::narrow_cast<char>(input->peek());
	}
	void BnfLexer::SkipChar() {
		colNo++;
		input->ignore();
	}
	char BnfLexer::GetChar() {
		colNo++;
		return gsl::narrow_cast<char>(input->get());
	}

	void BnfLexer::SkipWhitespace() {
		while(!IsInputEnd() && CharUtils::IsSpace(PeekChar())) {
			if(GetChar() == '\n') {
				lineStart += std::exchange(colNo, 0);
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
			// if the end of input was reached in the middle of regular expression, it is an error
			if(IsInputEnd()) {
				throw ParseError("unexpected end of file", GetInputPos());
			}

			// same for unexpected line endings
			if(PeekChar() == '\n') {
				throw ParseError("unexpected end of line", GetInputPos());
			}

			if(PeekChar() == regexDelim) {
				SkipChar();
				break;
			}
			tokText += GetChar();
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
				const auto msg = (std::ostringstream()
					<< "unexpected \'" << PeekChar() << '\''
				).str();
				throw ParseError(msg, GetInputPos());
			}
		}
		tokText += GetChar();
	}
	void BnfLexer::ParseToken() {
		// remove all whitespace from the input before parsing the token
		SkipWhitespace();

		const auto startCol = colNo; // save the starting column of the token
		tokText.clear(); // clear the token text buffer

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
		colCount = colNo - startCol;
	}

	SourceLocation BnfLexer::GetInputPos() const noexcept {
		if(IsTokenParsed()) {
			return SourceLocation(
				lineStart, colNo - colCount, colCount, lineNo
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