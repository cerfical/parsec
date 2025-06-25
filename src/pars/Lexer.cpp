#include "pars/Lexer.hpp"
#include "pars/ParseError.hpp"

#include "regex/Parser.hpp"

import parsec.text;

namespace parsec::pars {
    TokenKinds Lexer::parseToken() {
        skipWhitespace();
        resetParse();

        if(input_.isEof()) {
            return TokenKinds::Eof;
        }

        if(isIdentStart()) {
            return parseIdent();
        }

        if(isStringLiteralStart()) {
            return parseStringLiteral();
        }
        return parseOperator();
    }


    void Lexer::skipWhitespace() {
        while(true) {
            // nothing to skip, the input is empty
            if(input_.isEof()) {
                break;
            }

            if(text::isSpace(input_.peek())) { // space characters
                input_.skip();
            } else if(input_.skipIf("//")) { // single-line comments
                while(!input_.isEof() && input_.get() != '\n') {
                    // skip until the end of line or file
                }
            } else if(input_.skipIf("/*")) { // multi-line comments
                while(!input_.skipIf("*/")) {
                    // skip all characters until the end of comment token is encountered
                    input_.skip();
                }
            } else {
                break;
            }
        }
    }


    void Lexer::resetParse() {
        tokenStart_ = input_.pos().offset;
        tokenText_.clear();
    }


    TokenKinds Lexer::parseStringLiteral() {
        const auto delim = input_.get();
        while(true) {
            if(input_.peek() == '\n') {
                throw ParseError::misplacedChar(input_.pos(), '\n');
            }

            if(input_.skipIf(delim)) {
                break;
            }

            // automatically escape regex metachars in raw string literals
            const auto ch = input_.get();
            if(delim == '\'' && regex::Parser::isMetaChar(ch)) {
                tokenText_ += '\\';
            }
            tokenText_ += ch;

            /**
             * Provide only minimal handling of escape sequences:
             *  1. Allows for pattern delimiter (") to be included in the pattern itself
             *  2. Independence from specific escape sequences supported by regex patterns
             */
            if(delim == '"' && ch == '\\') {
                tokenText_ += input_.get();
            }
        }
        return TokenKinds::PatternString;
    }


    bool Lexer::isStringLiteralStart() const {
        if(input_.isEof()) {
            return false;
        }

        const auto ch = input_.peek();
        if(ch == '"' || ch == '\'') {
            return true;
        }
        return false;
    }


    TokenKinds Lexer::parseIdent() {
        while(isIdentStart() || text::isDigit(input_.peek())) {
            tokenText_ += input_.get();
        }
        return TokenKinds::Ident;
    }


    bool Lexer::isIdentStart() const {
        const auto ch = input_.peek();
        return !input_.isEof() && (text::isAlpha(ch) || ch == '-' || ch == '_');
    }


    TokenKinds Lexer::parseOperator() {
        TokenKinds kind = {};
        switch(input_.peek()) {
            case '{': kind = TokenKinds::LeftBrace; break;
            case '}': kind = TokenKinds::RightBrace; break;
            case '(': kind = TokenKinds::LeftParen; break;
            case ')': kind = TokenKinds::RightParen; break;
            case ';': kind = TokenKinds::Semicolon; break;
            case '=': kind = TokenKinds::Equals; break;
            case '|': kind = TokenKinds::Pipe; break;
            case '*': kind = TokenKinds::Star; break;
            case '+': kind = TokenKinds::Plus; break;
            case '?': kind = TokenKinds::QuestionMark; break;
            default:  {
                throw ParseError::invalidChar(input_.pos(), input_.peek());
            }
        }
        tokenText_ += input_.get();
        return kind;
    }
}
