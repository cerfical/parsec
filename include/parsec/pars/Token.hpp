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

        Token() noexcept = default;

        Token(std::string text, TokenKinds kind, const SourceLoc& loc) noexcept
            : text_(std::move(text)), loc_(loc), kind_(kind) {}


        /** @{ */
        /**
         * @brief Character sequence covered by the token.
         */
        const std::string& text() const noexcept {
            return text_;
        }


        /**
         * @brief Location of the token.
         */
        const SourceLoc& loc() const noexcept {
            return loc_;
        }


        /**
         * @brief Classification of the token.
         */
        TokenKinds kind() const noexcept {
            return kind_;
        }


        /**
         * @brief Check if the token is of the specified type.
         */
        template <TokenKinds K>
        bool is() const noexcept {
            return kind() == K;
        }
        /** @} */


    private:
        std::string text_;
        SourceLoc loc_;
        TokenKinds kind_ = {};
    };


    inline std::ostream& operator<<(std::ostream& out, const Token& tok) {
        out << '(' << tok.kind() << ": " << '"' << tok.text() << "\")";
        return out;
    }

}
