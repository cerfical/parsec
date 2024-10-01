#pragma once

#include "ExprNode.hpp"

namespace parsec::regex {

    class BinaryExpr : public ExprNode {
    public:

        BinaryExpr(NodePtr left, NodePtr right) noexcept
            : left_(std::move(left)), right_(std::move(right)) {}


        int atomCount() const noexcept override {
            return left_->atomCount() + right_->atomCount();
        }


        const ExprNode* left() const noexcept {
            return left_.get();
        }

        const ExprNode* right() const noexcept {
            return right_.get();
        }


    private:
        NodePtr left_;
        NodePtr right_;
    };

}
