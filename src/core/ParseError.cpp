#include "core/ParseError.hpp"

#include "util/char_util.hpp"
#include "util/string_util.hpp"

#include <format>

namespace parsec {
	ParseError ParseError::nameRedefine(const SourceLoc& loc) {
		return ParseError(loc, "name redefinition");
	}


	ParseError ParseError::reservedNameRedefine(const SourceLoc& loc) {
		return ParseError(loc, "trying to redefine a reserved name");
	}


	ParseError ParseError::emptyName(const SourceLoc& loc) {
		return ParseError(loc, "name must contain at least one letter");
	}

	
	ParseError ParseError::undefinedName(const SourceLoc& loc) {
		return ParseError(loc, "use of undefined name");
	}


	ParseError ParseError::patternConflict(const SourceLoc& loc, std::string_view pattern) {
		return ParseError(loc, std::format("pattern is in conflict with pattern \"{}\"", pattern));
	}


	ParseError ParseError::ruleConflict(const SourceLoc& loc, std::string_view rule) {
		return ParseError(loc, std::format("rule is in conflict with rule \"{}\"", rule));
	}


	ParseError ParseError::unexpectedEof(const SourceLoc& loc) {
		return ParseError(loc, "unexpected end of file");
	}


	ParseError ParseError::invalidChar(const SourceLoc& loc, char ch) {
		return ParseError(loc, std::format("invalid character '{}'", char_util::escape(ch)));
	}


	ParseError ParseError::emptyHexCharSeq(const SourceLoc& loc) {
		return ParseError(loc, "expected at least one hexadecimal digit");
	}


	ParseError ParseError::outOfOrderCharRange(const SourceLoc& loc) {
		return ParseError(loc, "character range is out of order");
	}


	ParseError ParseError::misplacedChar(const SourceLoc& loc, char ch) {
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
				msg = std::format("unexpected '{}'", char_util::escape(ch));
				break;
			}
		}
		return ParseError(loc, msg);
	}


	ParseError ParseError::misplacedToken(const SourceLoc& loc, std::string_view tok) {
		return ParseError(loc, std::format("unexpected \"{}\"", string_util::escape(tok)));
	}


	ParseError ParseError::unmatchedToken(const SourceLoc& loc, std::string_view tok) {
		return ParseError(loc, std::format("{} expected, but got", tok));
	}
}