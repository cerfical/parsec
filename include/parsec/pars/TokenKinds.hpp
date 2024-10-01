#pragma once

#include <ostream>

namespace parsec::pars {

    /**
     * @brief List of token types produced by @ref parsec::pars::Lexer "Lexer".
     */
    enum class TokenKinds {
        EmptyToken, /**< @brief Empty token. */
        Eof,        /**< @brief End of file. */

        Ident,         /**< @brief Identifier. */
        PatternString, /**< @brief Regex string pattern. */

        Star,         /**< @brief Asterisk symbol. */
        Plus,         /**< @brief Plus symbol. */
        QuestionMark, /**< @brief Question mark. */
        Pipe,         /**< @brief Vertical bar. */
        Semicolon,    /**< @brief Semicolon. */
        Equals,       /**< @brief Equals sign. */

        LeftBrace,  /**< @brief Opening curly brace. */
        RightBrace, /**< @brief Closing curly brace. */

        LeftParen, /**< @brief Opening parenthesis. */
        RightParen /**< @brief Closing parenthesis. */
    };


    inline std::ostream& operator<<(std::ostream& out, TokenKinds tok) {
        switch(tok) {
            case TokenKinds::EmptyToken: out << "EmptyToken"; break;
            case TokenKinds::Eof:        out << "Eof"; break;

            case TokenKinds::Ident:         out << "Ident"; break;
            case TokenKinds::PatternString: out << "PatternString"; break;

            case TokenKinds::Star:         out << "Star"; break;
            case TokenKinds::Plus:         out << "Plus"; break;
            case TokenKinds::QuestionMark: out << "QuestionMark"; break;
            case TokenKinds::Pipe:         out << "Pipe"; break;
            case TokenKinds::Semicolon:    out << "Semicolon"; break;
            case TokenKinds::Equals:       out << "Equals"; break;

            case TokenKinds::LeftBrace:  out << "LeftBrace"; break;
            case TokenKinds::RightBrace: out << "RightBrace"; break;

            case TokenKinds::LeftParen:  out << "LeftParen"; break;
            case TokenKinds::RightParen: out << "RightParen"; break;
        }
        return out;
    }

}
