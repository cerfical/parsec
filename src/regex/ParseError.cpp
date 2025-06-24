#include "regex/ParseError.hpp"

#include "util/chars.hpp"

#include <format>
#include <string>

namespace parsec::regex {

    ParseError ParseError::unexpectedEof(const SourceLoc& loc) {
        return { loc, "unexpected end of file" };
    }

    ParseError ParseError::emptyHexEscapeSeq(const SourceLoc& loc) {
        return { loc, "expected at least one hexadecimal digit" };
    }

    ParseError ParseError::outOfOrderCharRange(const SourceLoc& loc) {
        return { loc, "character range is out of order" };
    }

    ParseError ParseError::misplacedChar(const SourceLoc& loc, char ch) {
        std::string msg;
        switch(ch) {
            case ')':
            case '(': {
                msg = "unmatched parenthesis";
                break;
            }
            case '\n': {
                msg = "unexpected end of line";
                break;
            }
            default: {
                msg = std::format("unexpected '{}'", chars::escape(ch));
                break;
            }
        }
        return { loc, msg };
    }

}
