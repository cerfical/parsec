#include "pars/ParseError.hpp"

#include "util/chars.hpp"
#include "util/strings.hpp"

#include <format>
#include <string>
#include <string_view>

namespace parsec::pars {

    ParseError ParseError::unexpectedEof(const SourceLoc& eofLoc) {
        return { eofLoc, "unexpected end of file" };
    }

    ParseError ParseError::invalidChar(const SourceLoc& chLoc, char ch) {
        return { chLoc, std::format("invalid character '{}'", chars::escape(ch)) };
    }

    ParseError ParseError::misplacedChar(const SourceLoc& chLoc, char ch) {
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
        return { chLoc, msg };
    }

    ParseError ParseError::misplacedToken(const SourceLoc& tokLoc, std::string_view tokVal) {
        if(tokLoc) {
            return { tokLoc, std::format("unexpected \"{}\"", strings::escape(tokVal)) };
        }
        return unexpectedEof(tokLoc);
    }

    ParseError ParseError::unmatchedToken(const SourceLoc& foundLoc, std::string_view expectVal) {
        return { foundLoc, std::format("{} expected, but got{}", expectVal, foundLoc ? "" : " end of file") };
    }

}
