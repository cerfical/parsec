#include "pars/Parser.hpp"

#include "core/ParseError.hpp"

#include "pars/ast/AlternRule.hpp"
#include "pars/ast/ConcatRule.hpp"
#include "pars/ast/EmptyNode.hpp"
#include "pars/ast/EmptyRule.hpp"
#include "pars/ast/InlineToken.hpp"
#include "pars/ast/ListNode.hpp"
#include "pars/ast/NamedRule.hpp"
#include "pars/ast/NamedToken.hpp"
#include "pars/ast/OptionalRule.hpp"
#include "pars/ast/PlusRule.hpp"
#include "pars/ast/StarRule.hpp"
#include "pars/ast/SymbolAtom.hpp"
#include "pars/Lexer.hpp"

#include <sstream>

namespace parsec::pars {
    namespace {
        class ParserImpl {
        public:

            explicit ParserImpl(std::istream& input)
                : lexer_(input) {}

            NodePtr run() {
                return parseSpec();
            }

        private:
            NodePtr parseSpec() {
                auto spec = makeNode<EmptyNode>();
                while(!lexer_.isEof()) {
                    if(lexer_.skipIf("tokens")) {
                        spec = makeNode<ListNode>(std::move(spec), parseDefList(&ParserImpl::parseToken));
                    } else if(lexer_.skipIf("rules")) {
                        spec = makeNode<ListNode>(std::move(spec), parseDefList(&ParserImpl::parseRule));
                    } else {
                        const auto& tok = lexer_.peek();
                        throw ParseError::misplacedToken(tok.loc(), tok.text());
                    }
                }
                return spec;
            }


            NodePtr parseDefList(NodePtr (ParserImpl::*parseDef)(const Token&)) {
                auto defs = makeNode<EmptyNode>();
                expect<TokenKinds::LeftBrace>();

                while(!lexer_.skipIf(TokenKinds::RightBrace)) {
                    if(lexer_.peek().is<TokenKinds::Ident>()) {
                        const auto name = lexer_.lex();
                        expect<TokenKinds::Equals>();

                        auto def = (this->*parseDef)(name);
                        defs = makeNode<ListNode>(std::move(defs), std::move(def));
                    }

                    if(!lexer_.skipIf(TokenKinds::Semicolon)) {
                        const auto& tok = lexer_.peek();
                        throw ParseError::misplacedToken(tok.loc(), tok.text());
                    }
                }

                return defs;
            }


            NodePtr parseToken(const Token& name) {
                return makeNode<NamedToken>(name, expect<TokenKinds::PatternString>());
            }


            NodePtr parseRule(const Token& name) {
                return makeNode<NamedRule>(name, parseRuleExpr());
            }


            NodePtr parseRuleExpr() {
                return parseAltern();
            }


            NodePtr parseAltern() {
                auto lhs = parseConcat();
                while(lexer_.skipIf(TokenKinds::Pipe)) {
                    lhs = makeNode<AlternRule>(std::move(lhs), parseConcat());
                }
                return lhs;
            }


            NodePtr parseConcat() {
                auto lhs = parseRepeat();
                while(isAtom()) {
                    lhs = makeNode<ConcatRule>(std::move(lhs), parseRepeat());
                }
                return lhs;
            }


            NodePtr parseRepeat() {
                auto inner = parseAtom();
                while(true) {
                    if(lexer_.skipIf(TokenKinds::Star)) {
                        inner = makeNode<StarRule>(std::move(inner));
                    } else if(lexer_.skipIf(TokenKinds::Plus)) {
                        inner = makeNode<PlusRule>(std::move(inner));
                    } else if(lexer_.skipIf(TokenKinds::QuestionMark)) {
                        inner = makeNode<OptionalRule>(std::move(inner));
                    } else {
                        break;
                    }
                }
                return inner;
            }


            NodePtr parseAtom() {
                switch(lexer_.peek().kind()) {
                    case TokenKinds::Ident: {
                        return makeNode<SymbolAtom>(lexer_.lex());
                    }
                    case TokenKinds::LeftParen: {
                        const auto openParen = lexer_.lex();
                        if(lexer_.skipIf(TokenKinds::RightParen)) {
                            return makeNode<EmptyRule>();
                        }

                        auto subrule = parseRuleExpr();
                        if(!lexer_.skipIf(TokenKinds::RightParen)) {
                            throw ParseError::misplacedChar(openParen.loc(), '(');
                        }
                        return subrule;
                    }
                    case TokenKinds::PatternString: {
                        return makeNode<InlineToken>(lexer_.lex());
                    }
                    case TokenKinds::RightParen: {
                        throw ParseError::misplacedChar(lexer_.loc(), ')');
                    }
                    default: {
                        const auto& tok = lexer_.peek();
                        throw ParseError::misplacedToken(tok.loc(), tok.text());
                    }
                }
            }


            bool isAtom() {
                switch(lexer_.peek().kind()) {
                    case TokenKinds::Ident:
                    case TokenKinds::LeftParen:
                    case TokenKinds::PatternString: {
                        return true;
                    }
                    default: {
                        return false;
                    }
                }
            }


            template <TokenKinds K>
            Token expect() {
                if(const auto& peekTok = lexer_.peek(); !peekTok.is<K>()) {
                    throw ParseError::unmatchedToken(peekTok.loc(), describeToken(K));
                }
                return lexer_.lex();
            }


            static std::string describeToken(TokenKinds tok) {
                switch(tok) {
                    case TokenKinds::EmptyToken: return "empty token";
                    case TokenKinds::Eof:        return "end of file";

                    case TokenKinds::Ident:         return "identifier";
                    case TokenKinds::PatternString: return "string pattern";

                    case TokenKinds::Star:         return "'*'";
                    case TokenKinds::Plus:         return "'+'";
                    case TokenKinds::QuestionMark: return "'?'";
                    case TokenKinds::Pipe:         return "'|'";
                    case TokenKinds::Semicolon:    return "';'";
                    case TokenKinds::Equals:       return "'='";

                    case TokenKinds::LeftBrace:  return "'{'";
                    case TokenKinds::RightBrace: return "'}'";

                    case TokenKinds::LeftParen:  return "'('";
                    case TokenKinds::RightParen: return "')'";
                }
                return "";
            }


            Lexer lexer_;
        };
    }


    NodePtr Parser::parse(std::string_view input) {
        auto inputStream = std::istringstream(std::string(input));
        return parse(inputStream);
    }


    NodePtr Parser::parse(std::istream& input) {
        return ParserImpl(input).run();
    }
}
