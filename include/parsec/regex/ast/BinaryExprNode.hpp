#pragma once

#include "ExprNode.hpp"

namespace parsec::regex {

    /**
     * @brief Expression consisting of two subexpressions.
     */
    class BinaryExprNode : public ExprNode {
    public:

        BinaryExprNode(NodePtr left, NodePtr right)
            : left_(std::move(left)), right_(std::move(right)) {}

        int atomCount() const noexcept override {
            return left_->atomCount() + right_->atomCount();
        }


        /**
         * @brief Left subexpression.
         */
        const ExprNode* left() const noexcept {
            return left_.get();
        }


        /**
         * @brief Right subexpression.
         */
        const ExprNode* right() const noexcept {
            return right_.get();
        }


    private:
        NodePtr left_;
        NodePtr right_;
    };

}
