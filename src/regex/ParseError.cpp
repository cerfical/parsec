#include "regex/ParseError.hpp"

#include "text/chars.hpp"

#include <format>
#include <string>

namespace parsec::regex {

    ParseError ParseError::unexpectedEof(const scan::SourceLoc& loc) {
        return { loc, "unexpected end of file" };
    }

    ParseError ParseError::emptyHexEscapeSeq(const scan::SourceLoc& loc) {
        return { loc, "expected at least one hexadecimal digit" };
    }

    ParseError ParseError::outOfOrderCharRange(const scan::SourceLoc& loc) {
        return { loc, "character range is out of order" };
    }

    ParseError ParseError::misplacedChar(const scan::SourceLoc& loc, char ch) {
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
                msg = std::format("unexpected '{}'", text::chars::escape(ch));
                break;
            }
        }
        return { loc, msg };
    }

}
