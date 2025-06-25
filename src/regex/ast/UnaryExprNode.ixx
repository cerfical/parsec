module;

#include <utility>

export module parsec.regex:ast.UnaryExprNode;

import :ast.ExprNode;

namespace parsec::regex {

    /**
     * @brief Expression consisting of one subexpression.
     */
    export class UnaryExprNode : public ExprNode {
    public:

        explicit UnaryExprNode(NodePtr inner)
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
