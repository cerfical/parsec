#include "RegExParser.hpp"
#include <sstream>

namespace {
	using namespace parsec;

	SourceLocation MakeLocation(gsl::index colNo, std::size_t colCount) noexcept {
		return SourceLocation(0, colNo, colCount, 0);
	}
}

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

	SourceLocation RegExParser::GetInputPos() const noexcept {
		return MakeLocation(pos, 1);
	}
	bool RegExParser::IsInputEmpty() const noexcept {
		return input.size() == pos;
	}

	char RegExParser::PeekChar() const {
		if(!IsInputEmpty()) {
			return input[pos];
		}
		UnexpectedEof();
	}
	char RegExParser::GetChar() {
		if(!IsInputEmpty()) {
			return input[pos++];
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
					return static_cast<char>(ch);
				}
				throw ParseError("expected at least one hexadecimal digit", GetInputPos());
			}
		}
		throw ParseError("invalid escape sequence", MakeLocation(pos - 1, 2));
	}
	char RegExParser::ParseChar() {
		if(SkipCharIf('\\')) {
			return ParseEscapeSeq();
		} 
		return GetChar();
	}
	void RegExParser::ParseCharRange() {
		const auto startCol = GetInputPos().GetColumnNo();
		const auto low = ParseChar();

		auto e = static_cast<std::unique_ptr<RegExNode>>(std::make_unique<RegExChar>(low));
		if(SkipCharIf('-')) {
			if(PeekChar() != ']') {
				const auto high = ParseChar();
				if(low > high) {
					const auto loc = MakeLocation(startCol, GetInputPos().GetColumnNo() - startCol);
					throw ParseError("character range is out of order", loc);
				}

				for(char ch = low; ch < high; ) {
					auto right = std::make_unique<RegExChar>(++ch);
					e = std::make_unique<RegExAltern>(std::move(e), std::move(right));
				}
			} else {
				e = std::make_unique<RegExAltern>(std::move(e), std::make_unique<RegExChar>('-'));
			}
		}

		regex = std::move(e);
	}
	void RegExParser::ParseCharSet() {
		if(!SkipCharIf(']')) {
			auto left = (ParseCharRange(), std::move(regex));
			while(PeekChar() != ']') {
				auto right = (ParseCharRange(), std::move(regex));
				left = std::make_unique<RegExAltern>(std::move(left), std::move(right));
			}
			regex = std::move(left);
			SkipChar(); // skip ']'
		} else {
			regex = std::make_unique<RegExNil>();
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
				regex = std::make_unique<RegExNil>();
			}
		} else if(SkipCharIf('[')) {
			ParseCharSet();
		} else {
			regex = std::make_unique<RegExChar>(ParseChar());
		}
	}

	void RegExParser::ParseRepeat() {
		auto e = (ParseAtom(), std::move(regex));
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
		regex = std::move(e);
	}

	void RegExParser::ParseConcat() {
		auto left = (ParseRepeat(), std::move(regex));
		while(IsAtomStart()) {
			auto right = (ParseRepeat(), std::move(regex));
			left = std::make_unique<RegExConcat>(std::move(left), std::move(right));
		}
		regex = std::move(left);
	}
	
	void RegExParser::ParseAltern() {
		auto left = (ParseConcat(), std::move(regex));
		while(SkipCharIf('|')) {
			auto right = (ParseConcat(), std::move(regex));
			left = std::make_unique<RegExAltern>(std::move(left), std::move(right));
		}
		regex = std::move(left);
	}

	void RegExParser::ParseRegex() {
		ParseAltern();
	}

	std::unique_ptr<RegExNode> RegExParser::Parse(std::string_view regex) {
		input = regex; pos = 0;
		if(IsInputEmpty()) {
			this->regex = std::make_unique<RegExNil>();
		} else {
			ParseRegex();
			if(!IsInputEmpty()) {
				if(PeekChar() == ')') {
					throw ParseError("unmatched parenthesis", GetInputPos());
				}
				UnexpectedChar();
			}
		}
		return std::move(this->regex);
	}
}