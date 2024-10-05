#pragma once

#include "ExprNode.hpp"

namespace parsec::regex {

    /**
     * @brief Expression consisting of one subexpression.
     */
    class UnaryExpr : public ExprNode {
    public:

        explicit UnaryExpr(NodePtr inner) noexcept
            : inner_(std::move(inner)) {}


        int atomCount() const noexcept override {
            return inner_->atomCount();
        }


        /**
         * @brief Inner subexpression.
         */
        const ExprNode* inner() const noexcept {
            return inner_.get();
        }


    private:
        NodePtr inner_;
    };

}
