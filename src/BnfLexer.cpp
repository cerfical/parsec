#include "BnfLexer.hpp"
#include <sstream>

namespace parsec {
	void BnfLexer::Error(gsl::czstring msg) {
		// adjust the input location to point to the offending symbol
		auto loc = GetInputPos();
		loc.colNo += std::exchange(loc.colCount, 1);
		throw ParseError(msg, loc);
	}

	void BnfLexer::UnexpectedChar() {
		const auto ch = PeekChar();
		const auto msg = (std::ostringstream()
			<< "unexpected \'" << ch << '\''
		).str();
		Error(msg.c_str());
	}
	void BnfLexer::UnexpectedEof() {
		Error("unexpected end of file");
	}


	char BnfLexer::PeekChar() {
		if(!IsEof()) {
			return gsl::narrow_cast<char>(m_input->peek());
		}
		UnexpectedEof();
	}
	bool BnfLexer::SkipCharIf(char ch) {
		if(!IsEof()) {
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
		if(!IsEof()) {
			m_pos.colCount++;
			return gsl::narrow_cast<char>(m_input->get());
		}
		UnexpectedEof();
	}


	void BnfLexer::SkipWhitespace() {
		while(!IsEof() && CharUtils::IsSpace(PeekChar())) {
			if(GetChar() == '\n') {
				m_pos.startPos += std::exchange(m_pos.colNo, 0) + std::exchange(m_pos.colCount, 0);
				m_pos.lineNo++;
			}
		}
	}

	bool BnfLexer::IsIdentStart() {
		return !IsEof() && (
			CharUtils::IsAlnum(PeekChar())
				|| PeekChar() == '-'
				|| PeekChar() == '_'
		);
	}
	BnfTokenKinds BnfLexer::ParseIdent() {
		while(IsIdentStart()) {
			m_buf += GetChar();
		}
		return BnfTokenKinds::Ident;
	}

	bool BnfLexer::IsRegexStart() {
		return !IsEof() && (
			PeekChar() == '\'' || PeekChar() == '\"'
		);
	}
	BnfTokenKinds BnfLexer::ParseRegex() {
		const auto regexDelim = GetChar();
		while(true) {
			if(PeekChar() == '\n') {
				Error("unexpected end of line");
			}
			if(SkipCharIf(regexDelim)) {
				break;
			}
			
			const auto ch = GetChar();
			m_buf += ch;

			if(ch == '\\' && PeekChar() == regexDelim) {
				m_buf += GetChar();
			}
		}
		return BnfTokenKinds::Regex;
	}


	BnfTokenKinds BnfLexer::ParseOperator() {
		BnfTokenKinds tokKind = BnfTokenKinds::Eof;
		switch(PeekChar()) {
			case '{': tokKind = BnfTokenKinds::OpenBrace; break;
			case '}': tokKind = BnfTokenKinds::CloseBrace; break;
			case ';': tokKind = BnfTokenKinds::Semicolon; break;
			case '=': tokKind = BnfTokenKinds::Equals; break;
			case '|': tokKind = BnfTokenKinds::Pipe; break;
			default: UnexpectedChar();
		}
		m_buf += GetChar();
		return tokKind;
	}
	BnfTokenKinds BnfLexer::ParseToken() {
		// remove all whitespace from the input before parsing the token
		SkipWhitespace();
		m_pos.colNo += std::exchange(m_pos.colCount, 0);
		m_buf.clear(); // clear the text buffer

		// parse the token according to its type
		if(!IsEof()) {
			if(IsIdentStart()) {
				return ParseIdent();
			}
			if(IsRegexStart()) {
				return ParseRegex();
			}
			return ParseOperator();
		}
		return BnfTokenKinds::Eof;
	}


	bool BnfLexer::IsKeyword(std::string_view keyword) {
		if(Peek().GetKind() != BnfTokenKinds::Ident) {
			return false;
		}
		return Peek().GetText() == keyword;
	}
	bool BnfLexer::IsEof() {
		if(m_input) {
			// if the of end of file was reached, reset the eof bit
			if(m_input->peek() != std::char_traits<char>::eof()) {
				return false;
			}
			m_input->clear(m_input->rdstate() ^ std::ios_base::eofbit);
		}
		return true;
	}


	const BnfToken& BnfLexer::Peek() {
		if(!m_tok.has_value()) {
			const auto tokKind = ParseToken();
			m_tok = BnfToken(m_buf, tokKind, GetInputPos());
		}
		return *m_tok;
	}
	BnfToken BnfLexer::Lex() {
		// make sure that the token is parsed
		Peek();

		auto tok = std::move(m_tok.value());
		m_tok.reset();
		return tok;
	}
}