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

    std::ostream& operator<<(std::ostream& out, TokenKinds tok);

}
