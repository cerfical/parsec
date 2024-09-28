#include "regex/Parser.hpp"

#include "core/ParseError.hpp"
#include "core/TextScanner.hpp"

#include "regex/make.hpp"

#include "util/char_util.hpp"

#include <sstream>

namespace parsec::regex {
    namespace {
        class ParserImpl {
        public:

            explicit ParserImpl(std::istream& input)
                : input_(input) {}

            NodePtr run() {
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

        private:

            NodePtr parseExpr() {
                return parseAltern();
            }


            NodePtr parseAltern() {
                auto lhs = parseConcat();
                while(input_.skipIf('|')) {
                    lhs = altern(lhs, parseConcat());
                }
                return lhs;
            }


            NodePtr parseConcat() {
                auto lhs = parseRepeat();
                while(isAtom()) {
                    lhs = concat(lhs, parseRepeat());
                }
                return lhs;
            }


            NodePtr parseRepeat() {
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


            NodePtr parseAtom() {
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

                return atom(parseChar());
            }


            bool isAtom() const {
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


            NodePtr parseCharSet() {
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


            NodePtr parseCharRange() {
                // save the position and value of the lower bound of the possible character range
                auto rngLoc = input_.pos();
                const auto low = parseChar();

                // no char range, just a single character
                auto expr = atom(low);
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
                        expr = altern(expr, atom(static_cast<char>(ch)));
                    }
                } else {
                    expr = altern(expr, atom('-'));
                }

                return expr;
            }


            char parseChar() {
                if(input_.skipIf('\\')) {
                    return parseEscapeSeq();
                }
                return input_.get();
            }


            char parseEscapeSeq() {
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
                        if(char_util::isHexDigit(input_.peek())) {
                            auto ch = char_util::evalHexDigit(input_.get());
                            if(!input_.isEof() && char_util::isHexDigit(input_.peek())) {
                                ch = ch * HexBase + char_util::evalHexDigit(input_.get());
                            }
                            return static_cast<char>(ch);
                        }
                        throw ParseError::emptyHexEscapeSeq(input_.pos());
                    }
                    default: return ch;
                }
            }


            TextScanner input_;
        };
    }


    NodePtr Parser::parse(std::string_view input) {
        auto inputStream = std::istringstream(std::string(input));
        return parse(inputStream);
    }


    NodePtr Parser::parse(std::istream& input) {
        return ParserImpl(input).run();
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
}
