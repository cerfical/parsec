#include "RegExParser.hpp"
#include <sstream>

namespace parsec {
	SourceLocation RegExParser::GetInputPos() const noexcept {
		const auto loc = SourceLocation({
			.chars = CharRange({ .startPos = pos, .size = 1 })
		});
		return loc;
	}
	bool RegExParser::IsInputEmpty() const noexcept {
		return input.size() == pos;
	}

	char RegExParser::PeekChar() const noexcept {
		return input[pos];
	}
	char RegExParser::GetChar() noexcept {
		return input[pos++];
	}

	bool RegExParser::SkipCharIf(char ch) noexcept {
		if(IsInputEmpty()) {
			return false;
		}
		if(PeekChar() == ch) {
			SkipChar();
			return true;
		}
		return false;
	}
	void RegExParser::SkipChar() noexcept {
		GetChar();
	}

	bool RegExParser::IsAtomStart() const noexcept {
		if(IsInputEmpty()) {
			return false;
		}
		switch(PeekChar()) {
			case '*': case '|': case ')': return false;
			case '(': return true;
			default: break;
		}
		return true;
	}

	void RegExParser::ParseAtom() {
		const auto loc = GetInputPos();
		if(!IsAtomStart()) {
			if(!IsInputEmpty()) {
				const auto msg = (std::ostringstream()
					<< "unexpected \'" << PeekChar() << '\''
				).str();
				throw ParseError(msg, loc);
			}
			throw ParseError("unexpected end of string", loc);
		}
		if(SkipCharIf('(')) {
			ParseRegex();
			if(!SkipCharIf(')')) {
				throw ParseError("unmatched parenthesis", loc);
			}
		} else {
			regex = std::make_unique<RegExLiteral>(GetChar());
		}
	}

	void RegExParser::ParseStar() {
		ParseAtom();
		if(SkipCharIf('*')) {
			while(SkipCharIf('*'));
		}
	}

	void RegExParser::ParseConcat() {
		auto left = (ParseStar(), std::move(regex));
		while(IsAtomStart()) {
			auto right = (ParseStar(), std::move(regex));
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
}