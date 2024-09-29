#include "pars/Parser.hpp"

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

#include <spanstream>

namespace parsec::pars {
    NodePtr Parser::parseFrom(std::string_view str) {
        auto in = std::ispanstream(str);
        return parseFrom(in);
    }


    NodePtr Parser::parseFrom(std::istream& in) {
        return Parser(&in).parse();
    }


    NodePtr Parser::parse() {
        return parseSpec();
    }


    NodePtr Parser::parseSpec() {
        auto spec = makeNode<EmptyNode>();
        while(!lexer_.isEof()) {
            if(lexer_.skipIf("tokens")) {
                spec = makeNode<ListNode>(std::move(spec), parseDefList(&Parser::parseToken));
            } else if(lexer_.skipIf("rules")) {
                spec = makeNode<ListNode>(std::move(spec), parseDefList(&Parser::parseRule));
            } else {
                const auto& tok = lexer_.peek();
                throw ParseError::misplacedToken(tok.loc(), tok.text());
            }
        }
        return spec;
    }


    NodePtr Parser::parseDefList(NodePtr (Parser::*parseDef)(const Token&)) {
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


    NodePtr Parser::parseToken(const Token& name) {
        return makeNode<NamedToken>(name, expect<TokenKinds::PatternString>());
    }


    NodePtr Parser::parseRule(const Token& name) {
        return makeNode<NamedRule>(name, parseRuleExpr());
    }


    NodePtr Parser::parseRuleExpr() {
        return parseAltern();
    }


    NodePtr Parser::parseAltern() {
        auto lhs = parseConcat();
        while(lexer_.skipIf(TokenKinds::Pipe)) {
            lhs = makeNode<AlternRule>(std::move(lhs), parseConcat());
        }
        return lhs;
    }


    NodePtr Parser::parseConcat() {
        auto lhs = parseRepeat();
        while(isAtom()) {
            lhs = makeNode<ConcatRule>(std::move(lhs), parseRepeat());
        }
        return lhs;
    }


    NodePtr Parser::parseRepeat() {
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


    NodePtr Parser::parseAtom() {
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


    bool Parser::isAtom() {
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

}
