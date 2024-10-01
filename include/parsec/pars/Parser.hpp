#pragma once

#include "../core/ParseError.hpp"

#include "ast/Node.hpp"

#include "Lexer.hpp"

#include <istream>
#include <string_view>

namespace parsec::pars {

    class Parser {
    public:

        static NodePtr parseFrom(std::string_view str);

        static NodePtr parseFrom(std::istream& in);


        Parser(const Parser& other) = delete;
        Parser& operator=(const Parser& other) = delete;

        Parser(Parser&& other) noexcept = default;
        Parser& operator=(Parser&& other) noexcept = default;

        ~Parser() = default;


        Parser() noexcept = default;

        explicit Parser(std::istream* input) noexcept
            : lexer_(input) {}


        NodePtr parse();


    private:
        NodePtr parseSpec();
        NodePtr parseDefList(NodePtr (Parser::*parseDef)(const Token&));

        NodePtr parseToken(const Token& name);
        NodePtr parseRule(const Token& name);

        NodePtr parseRuleExpr();
        NodePtr parseAltern();
        NodePtr parseConcat();
        NodePtr parseRepeat();
        NodePtr parseAtom();

        bool isAtom();


        [[noreturn]]
        void misplacedToken() {
            const auto& tok = lexer_.peek();
            throw ParseError::misplacedToken(tok.loc(), tok.text());
        }

        template <TokenKinds K>
        Token expect() {
            if(const auto& peekTok = lexer_.peek(); !peekTok.is<K>()) {
                throw ParseError::unmatchedToken(peekTok.loc(), describeToken(K));
            }
            return lexer_.lex();
        }

        static consteval std::string_view describeToken(TokenKinds tok) noexcept {
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
        }


        Lexer lexer_;
    };

}
