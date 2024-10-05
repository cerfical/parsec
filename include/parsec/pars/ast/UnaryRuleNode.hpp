#pragma once

#include "Node.hpp"

namespace parsec::pars {

    /**
     * @brief Rule consisting of one subrule.
     */
    class UnaryRuleNode : public Node {
    public:

        explicit UnaryRuleNode(NodePtr inner) noexcept
            : inner_(std::move(inner)) {}


        /** @{ */
        /**
         * @brief Inner subrule.
         */
        const Node* inner() const noexcept {
            return inner_.get();
        }
        /** @} */


    private:
        NodePtr inner_;
    };

}
