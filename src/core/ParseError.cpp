#include "core/ParseError.hpp"

#include "core/SourceLoc.hpp"

#include "util/char_util.hpp"
#include "util/string_util.hpp"

#include <format>
#include <string>
#include <string_view>

namespace parsec {
    ParseError ParseError::nameRedefine(const SourceLoc& nameLoc) {
        return { nameLoc, "name redefinition" };
    }

    ParseError ParseError::reservedNameRedefine(const SourceLoc& nameLoc) {
        return { nameLoc, "trying to redefine a reserved name" };
    }

    ParseError ParseError::emptyName(const SourceLoc& nameLoc) {
        return { nameLoc, "name must contain at least one letter" };
    }

    ParseError ParseError::undefinedName(const SourceLoc& nameRefLoc) {
        return { nameRefLoc, "use of undefined name" };
    }


    ParseError ParseError::patternConflict(const SourceLoc& pat1Loc, std::string_view pat2Name) {
        return { pat1Loc, std::format("pattern is in conflict with pattern \"{}\"", pat2Name) };
    }

    ParseError ParseError::ruleConflict(const SourceLoc& rule1Loc, std::string_view rule2Name) {
        return { rule1Loc, std::format("rule is in conflict with rule \"{}\"", rule2Name) };
    }


    ParseError ParseError::unexpectedEof(const SourceLoc& eofLoc) {
        return { eofLoc, "unexpected end of file" };
    }

    ParseError ParseError::invalidChar(const SourceLoc& chLoc, char ch) {
        return { chLoc, std::format("invalid character '{}'", char_util::escape(ch)) };
    }


    ParseError ParseError::emptyHexEscapeSeq(const SourceLoc& seqLoc) {
        return { seqLoc, "expected at least one hexadecimal digit" };
    }

    ParseError ParseError::outOfOrderCharRange(const SourceLoc& rngLoc) {
        return { rngLoc, "character range is out of order" };
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
                msg = std::format("unexpected '{}'", char_util::escape(ch));
                break;
            }
        }
        return { chLoc, msg };
    }

    ParseError ParseError::misplacedToken(const SourceLoc& tokLoc, std::string_view tokVal) {
        return { tokLoc, std::format("unexpected \"{}\"", string_util::escape(tokVal)) };
    }

    ParseError ParseError::unmatchedToken(const SourceLoc& foundLoc, std::string_view expectVal) {
        return { foundLoc, std::format("{} expected, but got", expectVal) };
    }
}
