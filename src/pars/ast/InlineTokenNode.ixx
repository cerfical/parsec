module;

#include <utility>

export module parsec.pars:ast.InlineTokenNode;

import :ast.Node;
import :Token;

namespace parsec::pars {

    /**
     * @brief Definition of an inline token without a name.
     */
    export class InlineTokenNode : public Node {
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
