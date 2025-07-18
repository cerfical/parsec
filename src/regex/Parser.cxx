module;

#include <string>

module parsec.regex;

import parsec.text;
import parsec.scan;

namespace parsec::regex {

    namespace {
        NodePtr atomFromChar(char ch) {
            return atom(std::string(1, ch));
        }
    }

    bool Parser::isMetaChar(char ch) {
        switch(ch) {
            case '|':
            case '*':
            case '?':
            case '+':
            case '(':
            case ')':
            case '[':
            case ']':
            case '\\': {
                return true;
            }
            default: {
                return false;
            }
        }
    }


    NodePtr Parser::parse() {
        // no input, nothing to parse
        if(input_.isEof()) {
            return empty();
        }

        auto expr = parseExpr();
        if(!input_.isEof()) {
            throw ParseError::misplacedChar(input_.pos(), input_.peek());
        }
        return expr;
    }


    NodePtr Parser::parseExpr() {
        try {
            return parseAltern();
        } catch(const scan::UnexpectedEofError& e) {
            throw ParseError::unexpectedEof(e.loc());
        }
    }


    NodePtr Parser::parseAltern() {
        auto lhs = parseConcat();
        while(input_.skipIf('|')) {
            lhs = altern(lhs, parseConcat());
        }
        return lhs;
    }


    NodePtr Parser::parseConcat() {
        auto lhs = parseRepeat();
        while(isAtom()) {
            lhs = concat(lhs, parseRepeat());
        }
        return lhs;
    }


    NodePtr Parser::parseRepeat() {
        auto expr = parseAtom();
        while(true) {
            if(input_.skipIf('*')) {
                expr = starClosure(expr);
            } else if(input_.skipIf('?')) {
                expr = optional(expr);
            } else if(input_.skipIf('+')) {
                expr = plusClosure(expr);
            } else {
                break;
            }
        }
        return expr;
    }


    NodePtr Parser::parseAtom() {
        if(!isAtom()) {
            throw ParseError::misplacedChar(input_.pos(), input_.peek());
        }

        if(const auto openParen = input_.pos(); input_.skipIf('(')) {
            // empty parenthesized expression
            if(input_.skipIf(')')) {
                return empty();
            }

            auto expr = parseExpr();
            if(!input_.skipIf(')')) {
                throw ParseError::misplacedChar(openParen, '(');
            }
            return expr;
        }

        if(input_.skipIf('[')) {
            return parseCharSet();
        }

        return atomFromChar(parseChar());
    }


    bool Parser::isAtom() const {
        if(!input_.isEof()) {
            switch(input_.peek()) {
                case '*':
                case '|':
                case ')':
                case ']':
                case '?':
                case '+':  break;
                case '(':
                case '[':
                case '\\':
                default:   return true;
            }
        }
        return false;
    }


    NodePtr Parser::parseCharSet() {
        // empty character set
        if(input_.skipIf(']')) {
            return empty();
        }

        auto lhs = parseCharRange();
        while(input_.peek() != ']') {
            lhs = altern(lhs, parseCharRange());
        }
        input_.skip(); // skip ']'
        return lhs;
    }


    NodePtr Parser::parseCharRange() {
        // save the position and value of the lower bound of the possible character range
        auto rngLoc = input_.pos();
        const auto low = parseChar();

        // no char range, just a single character
        auto expr = atomFromChar(low);
        if(!input_.skipIf('-')) {
            return expr;
        }

        // string of the form 'l-h' is a character range
        if(input_.peek() != ']') {
            const auto high = parseChar();
            if(low > high) {
                const auto& inputPos = input_.pos();
                rngLoc.colCount = inputPos.offset - rngLoc.offset;

                throw ParseError::outOfOrderCharRange(rngLoc);
            }

            for(auto ch = low + 1; ch <= high; ch++) {
                expr = altern(expr, atomFromChar(static_cast<char>(ch)));
            }
        } else {
            expr = altern(expr, atomFromChar('-'));
        }

        return expr;
    }


    char Parser::parseChar() {
        if(input_.skipIf('\\')) {
            return parseEscapeSeq();
        }
        return input_.get();
    }


    char Parser::parseEscapeSeq() {
        const auto ch = input_.get();
        switch(ch) {
            case '0': return '\0';
            case 'a': return '\a';
            case 'b': return '\b';
            case 'f': return '\f';
            case 'n': return '\n';
            case 'r': return '\r';
            case 't': return '\t';
            case 'v': return '\v';
            case 'x': {
                static constexpr int HexBase = 16;
                if(text::isHexDigit(input_.peek())) {
                    auto ch = text::evalHexDigit(input_.get());
                    if(!input_.isEof() && text::isHexDigit(input_.peek())) {
                        ch = ch * HexBase + text::evalHexDigit(input_.get());
                    }
                    return static_cast<char>(ch);
                }
                throw ParseError::emptyHexEscapeSeq(input_.pos());
            }
            default: return ch;
        }
    }

}
