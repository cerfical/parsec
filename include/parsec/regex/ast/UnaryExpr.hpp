#pragma once

#include "ExprNode.hpp"

namespace parsec::regex {

    class UnaryExpr : public ExprNode {
    public:

        explicit UnaryExpr(NodePtr inner) noexcept
            : inner_(std::move(inner)) {}


        int atomCount() const noexcept override {
            return inner_->atomCount();
        }


        const ExprNode* inner() const noexcept {
            return inner_.get();
        }


    private:
        NodePtr inner_;
    };

}
