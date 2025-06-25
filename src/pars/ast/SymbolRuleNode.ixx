module;

#include <utility>

export module parsec.pars:ast.SymbolRuleNode;

import :ast.Node;
import :Token;

namespace parsec::pars {

    /**
     * @brief Trivial rule consisting of a single symbolic name.
     */
    export class SymbolRuleNode : public Node {
    public:

        explicit SymbolRuleNode(Token value)
            : value_(std::move(value)) {}

        void accept(NodeVisitor& visitor) const override;


        /**
         * @brief Value of the symbolic name.
         */
        const Token& value() const noexcept {
            return value_;
        }


    private:
        Token value_;
    };

}
