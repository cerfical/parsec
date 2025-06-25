module;

#include <utility>

module parsec.pars;

namespace parsec::pars {
    NodePtr Parser::parseSpec() {
        auto spec = makeNode<EmptyNode>();
        while(!lexer_.isEof()) {
            if(lexer_.skipIf("tokens")) {
                spec = makeNode<ListNode>(std::move(spec), parseDefList(&Parser::parseToken));
            } else if(lexer_.skipIf("rules")) {
                spec = makeNode<ListNode>(std::move(spec), parseDefList(&Parser::parseRule));
            } else {
                misplacedToken();
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
                misplacedToken();
            }
        }

        return defs;
    }


    NodePtr Parser::parseToken(const Token& name) {
        return makeNode<NamedTokenNode>(name, expect<TokenKinds::PatternString>());
    }


    NodePtr Parser::parseRule(const Token& name) {
        return makeNode<NamedRuleNode>(name, parseRuleExpr());
    }


    NodePtr Parser::parseRuleExpr() {
        return parseAltern();
    }


    NodePtr Parser::parseAltern() {
        auto lhs = parseConcat();
        while(lexer_.skipIf(TokenKinds::Pipe)) {
            lhs = makeNode<AlternRuleNode>(std::move(lhs), parseConcat());
        }
        return lhs;
    }


    NodePtr Parser::parseConcat() {
        auto lhs = parseRepeat();
        while(isAtom()) {
            lhs = makeNode<ConcatRuleNode>(std::move(lhs), parseRepeat());
        }
        return lhs;
    }


    NodePtr Parser::parseRepeat() {
        auto inner = parseAtom();
        while(true) {
            if(lexer_.skipIf(TokenKinds::Star)) {
                inner = makeNode<StarRuleNode>(std::move(inner));
            } else if(lexer_.skipIf(TokenKinds::Plus)) {
                inner = makeNode<PlusRuleNode>(std::move(inner));
            } else if(lexer_.skipIf(TokenKinds::QuestionMark)) {
                inner = makeNode<OptionalRuleNode>(std::move(inner));
            } else {
                break;
            }
        }
        return inner;
    }


    NodePtr Parser::parseAtom() {
        switch(lexer_.peek().kind()) {
            case TokenKinds::Ident: {
                return makeNode<SymbolRuleNode>(lexer_.lex());
            }
            case TokenKinds::LeftParen: {
                const auto openParen = lexer_.lex();
                if(lexer_.skipIf(TokenKinds::RightParen)) {
                    return makeNode<EmptyRuleNode>();
                }

                auto subrule = parseRuleExpr();
                if(!lexer_.skipIf(TokenKinds::RightParen)) {
                    throw ParseError::misplacedChar(openParen.loc(), '(');
                }
                return subrule;
            }
            case TokenKinds::PatternString: {
                return makeNode<InlineTokenNode>(lexer_.lex());
            }
            case TokenKinds::RightParen: {
                throw ParseError::misplacedChar(lexer_.loc(), ')');
            }
            default: {
                misplacedToken();
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
