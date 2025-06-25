#include "pars/ParseError.hpp"

#include <format>
#include <string>
#include <string_view>

import parsec.text;

namespace parsec::pars {

    ParseError ParseError::unexpectedEof(const scan::SourceLoc& eofLoc) {
        return { eofLoc, "unexpected end of file" };
    }

    ParseError ParseError::invalidChar(const scan::SourceLoc& chLoc, char ch) {
        return { chLoc, std::format("invalid character '{}'", text::escape(ch)) };
    }

    ParseError ParseError::misplacedChar(const scan::SourceLoc& chLoc, char ch) {
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
                msg = std::format("unexpected '{}'", text::escape(ch));
                break;
            }
        }
        return { chLoc, msg };
    }

    ParseError ParseError::misplacedToken(const scan::SourceLoc& tokLoc, std::string_view tokVal) {
        if(tokLoc) {
            return { tokLoc, std::format("unexpected \"{}\"", text::escape(tokVal)) };
        }
        return unexpectedEof(tokLoc);
    }

    ParseError ParseError::unmatchedToken(const scan::SourceLoc& foundLoc, std::string_view expectVal) {
        return { foundLoc, std::format("{} expected, but got{}", expectVal, foundLoc ? "" : " end of file") };
    }

}
