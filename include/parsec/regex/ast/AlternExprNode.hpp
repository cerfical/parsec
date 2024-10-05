#pragma once

#include "BinaryExprNode.hpp"

namespace parsec::regex {

    /**
     * @brief Alternation of two expressions.
     */
    class AlternExprNode : public BinaryExprNode {
    public:

        using BinaryExprNode::BinaryExprNode;


        /** @{ */
        void accept(NodeVisitor& visitor) const override;

        bool isNullable() const noexcept override {
            return left()->isNullable() || right()->isNullable();
        }
        /** @} */
    };

}
