#include "err.hpp"

#include "core/ParseError.hpp"
#include <format>

namespace parsec::err {
	void unexpectedEof(const SourceLoc& eofLoc) {
		throw ParseError(eofLoc, "unexpected end of file");
	}


	void invalidChar(const SourceLoc& charLoc, char ch) {
		throw ParseError(charLoc, std::format("invalid character '{}'", ch));
	}


	void emptyHexCharSeq(const SourceLoc& hexSeqLoc) {
		throw ParseError(hexSeqLoc, "expected at least one hexadecimal digit");
	}


	void outOfOrderCharRange(const SourceLoc& charRangeLoc) {
		throw ParseError(charRangeLoc, "character range is out of order");
	}

	
	void misplacedChar(const SourceLoc& charLoc, char ch) {
		std::string msg;
		switch(ch) {
			case ')': case '(': {
				msg = "unmatched parenthesis";
				break;
			}
			case '\n': {
				msg = "unexpected end of line";
				break;
			}
			default: {
				msg = std::format("unexpected '{}'", ch);
				break;
			}
		}
		throw ParseError(charLoc, msg);
	}


	void misplacedToken(const SourceLoc& tokLoc, std::string_view tokText) {
		throw ParseError(tokLoc, std::format("unexpected \"{}\"", tokText));
	}


	void unmatchedToken(const SourceLoc& loc, std::string_view expect, std::string_view got) {
		throw ParseError(loc, std::format("expected \"{}\", but got \"{}\"", expect, got));
	}
}