#pragma once

#include "../core/SourceLoc.hpp"

#include "TokenKinds.hpp"

#include <ostream>
#include <string>
#include <utility>

namespace parsec::pars {

    /**
     * @brief Character sequence classified according to some rules.
     */
    class Token {
    public:

        Token() = default;

        Token(std::string text, TokenKinds kind, const SourceLoc& loc)
            : text_(std::move(text)), loc_(loc), kind_(kind) {}


        /** @{ */
        /**
         * @brief Character sequence covered by the token.
         */
        const std::string& text() const {
            return text_;
        }


        /**
         * @brief Location of the token.
         */
        const SourceLoc& loc() const {
            return loc_;
        }


        /**
         * @brief Classification of the token.
         */
        TokenKinds kind() const {
            return kind_;
        }


        /**
         * @brief Check if the token is of the specified type.
         */
        template <TokenKinds K>
        bool is() const {
            return kind() == K;
        }
        /** @} */



    private:
        std::string text_;
        SourceLoc loc_;
        TokenKinds kind_ = {};
    };

    std::ostream& operator<<(std::ostream& out, const Token& tok);

}
