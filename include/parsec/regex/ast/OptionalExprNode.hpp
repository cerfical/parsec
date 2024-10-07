#pragma once

#include "UnaryExprNode.hpp"

namespace parsec::regex {

    /**
     * @brief Denotes an 'optional' expresion.
     */
    class OptionalExprNode : public UnaryExprNode {
    public:

        using UnaryExprNode::UnaryExprNode;


        void accept(NodeVisitor& visitor) const override;

        bool isNullable() const noexcept override {
            return true;
        }
    };

}
