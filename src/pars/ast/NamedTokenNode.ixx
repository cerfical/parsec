module;

#include <utility>

export module parsec.pars:ast.NamedTokenNode;

import :ast.Node;
import :Token;

namespace parsec::pars {

    /**
     * @brief Definition of a named token.
     */
    export class NamedTokenNode : public Node {
    public:

        NamedTokenNode(Token name, Token pattern)
            : name_(std::move(name)), pattern_(std::move(pattern)) {}

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


    private:
        Token name_;
        Token pattern_;
    };

}
