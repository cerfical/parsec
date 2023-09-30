#include "RegExParser.hpp"
#include <sstream>

namespace parsec {
	void RegExParser::UnexpectedChar() const {
		const auto ch = PeekChar();
		const auto msg = (std::ostringstream()
			<< "unexpected \'" << ch << '\''
		).str();

		throw ParseError(msg, GetInputPos());
	}
	void RegExParser::UnexpectedEof() const {
		throw ParseError("unexpected end of string", GetInputPos());
	}


	SourceLoc RegExParser::GetInputPos() const noexcept {
		return { .colNo = m_pos, .colCount = 1 };
	}
	bool RegExParser::IsInputEmpty() const noexcept {
		return m_input.size() == m_pos;
	}


	char RegExParser::PeekChar() const {
		if(!IsInputEmpty()) {
			return m_input[m_pos];
		}
		UnexpectedEof();
	}
	char RegExParser::GetChar() {
		if(!IsInputEmpty()) {
			return m_input[m_pos++];
		}
		UnexpectedEof();
	}

	bool RegExParser::SkipCharIf(char ch) noexcept {
		if(!IsInputEmpty()) {
			if(PeekChar() == ch) {
				SkipChar();
				return true;
			}
		}
		return false;
	}
	void RegExParser::SkipChar() {
		GetChar();
	}


	bool RegExParser::IsAtomStart() const noexcept {
		if(!IsInputEmpty()) {
			switch(PeekChar()) {
				case '*': case '|': case ')': case ']': case '?': case '+': break;
				case '(': case '[': case '\\': default: return true;
			}	
		}
		return false;
	}


	char RegExParser::ParseEscapeSeq() {
		switch(PeekChar()) {
			case '*': case '|': case '(': case ')': case '[': case ']': case '?': case '+':
			case '\\': case '\'': case '\"': { return GetChar(); }
			case '0': { SkipChar(); return '\0'; }
			case 'a': { SkipChar(); return '\a'; }
			case 'b': { SkipChar(); return '\b'; }
			case 'f': { SkipChar(); return '\f'; }
			case 'n': { SkipChar(); return '\n'; }
			case 'r': { SkipChar(); return '\r'; }
			case 't': { SkipChar(); return '\t'; }
			case 'v': { SkipChar(); return '\v'; }
			case 'x': {
				SkipChar();
				if(CharUtils::IsHexDigit(PeekChar())) {
					auto ch = CharUtils::EvalHexDigit(GetChar());
					if(!IsInputEmpty() && CharUtils::IsHexDigit(PeekChar())) {
						ch = ch * 16 + CharUtils::EvalHexDigit(GetChar());
					}
					return gsl::narrow_cast<char>(ch);
				}
				throw ParseError("expected at least one hexadecimal digit", GetInputPos());
			}
		}
		throw ParseError("invalid escape sequence",
			{ .colNo = m_pos - 1, .colCount = 2 }
		);
	}
	char RegExParser::ParseChar() {
		if(SkipCharIf('\\')) {
			return ParseEscapeSeq();
		} 
		return GetChar();
	}
	void RegExParser::ParseCharRange() {
		const auto startPos = m_pos;
		const auto low = ParseChar();

		std::unique_ptr<RegExNode> e = std::make_unique<RegExChar>(low);
		if(SkipCharIf('-')) {
			if(PeekChar() != ']') {
				const auto high = ParseChar();
				if(low > high) {
					throw ParseError("character range is out of order",
						{ .colNo = startPos, .colCount = m_pos - startPos }
					);
				}

				for(char ch = low; ch < high; ) {
					auto right = std::make_unique<RegExChar>(++ch);
					e = std::make_unique<RegExAltern>(std::move(e), std::move(right));
				}
			} else {
				e = std::make_unique<RegExAltern>(std::move(e), std::make_unique<RegExChar>('-'));
			}
		}

		m_regex = std::move(e);
	}
	void RegExParser::ParseCharSet() {
		if(!SkipCharIf(']')) {
			auto left = (ParseCharRange(), std::move(m_regex));
			while(PeekChar() != ']') {
				auto right = (ParseCharRange(), std::move(m_regex));
				left = std::make_unique<RegExAltern>(std::move(left), std::move(right));
			}
			SkipChar(); // skip ']'
			m_regex = std::move(left);
		} else {
			m_regex = std::make_unique<RegExNil>();
		}
	}

	void RegExParser::ParseAtom() {
		if(!IsAtomStart()) {
			UnexpectedChar();
		}
		
		if(const auto openParen = GetInputPos(); SkipCharIf('(')) {
			if(!SkipCharIf(')')) {
				ParseRegex();
				if(!SkipCharIf(')')) {
					throw ParseError("unmatched parenthesis", openParen);
				}
			} else {
				m_regex = std::make_unique<RegExNil>();
			}
		} else if(SkipCharIf('[')) {
			ParseCharSet();
		} else {
			m_regex = std::make_unique<RegExChar>(ParseChar());
		}
	}

	void RegExParser::ParseRepeat() {
		auto e = (ParseAtom(), std::move(m_regex));
		while(true) {
			if(SkipCharIf('*')) {
				e = std::make_unique<RegExStar>(std::move(e));
			} else if(SkipCharIf('?')) {
				e = std::make_unique<RegExOpt>(std::move(e));
			} else if(SkipCharIf('+')) {
				e = std::make_unique<RegExPlus>(std::move(e));
			} else {
				break;
			}
		}
		m_regex = std::move(e);
	}

	void RegExParser::ParseConcat() {
		auto left = (ParseRepeat(), std::move(m_regex));
		while(IsAtomStart()) {
			auto right = (ParseRepeat(), std::move(m_regex));
			left = std::make_unique<RegExConcat>(std::move(left), std::move(right));
		}
		m_regex = std::move(left);
	}
	
	void RegExParser::ParseAltern() {
		auto left = (ParseConcat(), std::move(m_regex));
		while(SkipCharIf('|')) {
			auto right = (ParseConcat(), std::move(m_regex));
			left = std::make_unique<RegExAltern>(std::move(left), std::move(right));
		}
		m_regex = std::move(left);
	}

	void RegExParser::ParseRegex() {
		ParseAltern();
	}


	std::unique_ptr<RegExNode> RegExParser::Parse(std::string_view regex) {
		m_input = regex; m_pos = 0;
		if(IsInputEmpty()) {
			m_regex = std::make_unique<RegExNil>();
		} else {
			ParseRegex();
			if(!IsInputEmpty()) {
				if(PeekChar() == ')') {
					throw ParseError("unmatched parenthesis", GetInputPos());
				}
				UnexpectedChar();
			}
		}
		return std::move(m_regex);
	}
}