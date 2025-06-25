module;

#include <ostream>
#include <string>
#include <utility>

export module parsec.pars:Token;

import parsec.scan;

import :TokenKinds;

namespace parsec::pars {

    /**
     * @brief Character sequence classified according to some rules.
     */
    export class Token {
    public:

        Token() = default;


        Token(std::string text, TokenKinds kind, const scan::SourceLoc& loc)
            : text_(std::move(text)), loc_(loc), kind_(kind) {}


        /**
         * @brief Character sequence covered by the token.
         */
        const std::string& text() const noexcept {
            return text_;
        }


        /**
         * @brief Location of the token.
         */
        const scan::SourceLoc& loc() const noexcept {
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


    private:
        std::string text_;
        scan::SourceLoc loc_;
        TokenKinds kind_ = {};
    };


    export std::ostream& operator<<(std::ostream& out, const Token& tok) {
        out << '(' << tok.kind() << ": " << '"' << tok.text() << "\")";
        return out;
    }

}
