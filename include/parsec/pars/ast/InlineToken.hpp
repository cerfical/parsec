#pragma once

#include "../Token.hpp"

#include "Node.hpp"

namespace parsec::pars {

    /**
     * @brief Definition of an inline token without a name.
     */
    class InlineToken : public Node {
    public:

        explicit InlineToken(Token pattern) noexcept
            : pattern_(std::move(pattern)) {}


        void accept(NodeVisitor& visitor) const override;


        /**
         * @brief String pattern defining the token.
         */
        const Token& pattern() const noexcept {
            return pattern_;
        }


    private:
        Token pattern_;
    };

}
