#pragma once

#include "UnaryExprNode.hpp"

namespace parsec::regex {

    /**
     * @brief Denotes an application of the Kleene star operator to an expression.
     */
    class StarClosureNode : public UnaryExprNode {
    public:

        using UnaryExprNode::UnaryExprNode;


        /** @{ */
        void accept(NodeVisitor& visitor) const override;

        bool isNullable() const noexcept override {
            return true;
        }
        /** @} */
    };

}
