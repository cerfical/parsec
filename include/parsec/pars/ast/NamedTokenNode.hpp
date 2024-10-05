#pragma once

#include "../Token.hpp"
#include "Node.hpp"

namespace parsec::pars {

    /**
     * @brief Definition of a named token.
     */
    class NamedTokenNode : public Node {
    public:

        NamedTokenNode(Token name, Token pattern) noexcept
            : name_(std::move(name)), pattern_(std::move(pattern)) {}


        /** @{ */
        void accept(NodeVisitor& visitor) const override;


        /**
         * @brief Name of the token.
         */
        const Token& name() const noexcept {
            return name_;
        }


        /**
         * @brief String pattern defining the token.
         */
        const Token& pattern() const noexcept {
            return pattern_;
        }
        /** @} */


    private:
        Token name_;
        Token pattern_;
    };

}
