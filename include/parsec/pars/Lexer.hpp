#pragma once

#include "../core/TextScanner.hpp"
#include "Token.hpp"

#include <istream>
#include <optional>
#include <string>
#include <string_view>

namespace parsec::pars {

    /**
     * @brief Breaks input text into a sequence of Token%s.
     */
    class Lexer {
    public:

        Lexer() = default;

        Lexer(const Lexer& other) = delete;
        Lexer& operator=(const Lexer& other) = delete;

        Lexer(Lexer&& other) noexcept = default;
        Lexer& operator=(Lexer&& other) noexcept = default;

        ~Lexer() = default;


        /** @{ */
        /**
         * @brief Construct a new lexer that operates on an input stream.
         */
        explicit Lexer(std::istream* input)
            : input_(input) {}


        /**
         * @brief Perform the input analysis and extract the next token.
         */
        Token lex() {
            if(!token_) {
                token_ = nextToken();
            }

            auto tok = std::move(token_.value());
            token_.reset();
            return tok;
        }


        /**
         * @brief Check the next token without extracting it.
         */
        const Token& peek() {
            if(!token_) {
                token_ = nextToken();
            }
            return *token_;
        }


        /**
         * @brief Check whether the end of input has been reached.
         */
        bool isEof() {
            return peek().is<TokenKinds::Eof>();
        }


        /**
         * @brief Location of the lexer in the input stream.
         */
        SourceLoc loc() const {
            const auto& inputPos = input_.pos();
            return {
                .offset = tokenStart_,
                .colCount = inputPos.offset - tokenStart_,
                .line = inputPos.line,
            };
        }


        /**
         * @brief Remove a token from the input if it is of the specified type.
         * @returns @c true if a skip has taken place, @c false otherwise.
         */
        bool skipIf(TokenKinds tok) {
            if(peek().kind() == tok) {
                skip();
                return true;
            }
            return false;
        }


        /**
         * @brief Remove a token from the input if its text matches specified text.
         * @returns @c true if a skip has taken place, @c false otherwise.
         */
        bool skipIf(std::string_view tok) {
            if(peek().text() == tok) {
                skip();
                return true;
            }
            return false;
        }


        /**
         * @brief Remove the next token from the input.
         */
        void skip() {
            lex();
        }
        /** @} */


    private:
        Token nextToken() {
            const auto kind = parseToken();
            return Token(tokenText_, kind, loc());
        }

        void skipWhitespace();
        void resetParse();

        TokenKinds parseStringLiteral();
        bool isStringLiteralStart() const;

        TokenKinds parseIdent();
        bool isIdentStart() const;

        TokenKinds parseOperator();
        TokenKinds parseToken();

        int tokenStart_ = {};
        std::optional<Token> token_;
        std::string tokenText_;

        TextScanner input_;
    };

}
