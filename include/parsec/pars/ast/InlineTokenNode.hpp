#pragma once

#include "../Token.hpp"
#include "Node.hpp"

namespace parsec::pars {

    /**
     * @brief Definition of an inline token without a name.
     */
    class InlineTokenNode : public Node {
    public:

        explicit InlineTokenNode(Token pattern)
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
