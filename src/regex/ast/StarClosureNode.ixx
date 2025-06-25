export module parsec.regex:ast.StarClosureNode;

import :ast.UnaryExprNode;

namespace parsec::regex {

    /**
     * @brief Denotes an application of the Kleene star operator to an expression.
     */
    export class StarClosureNode : public UnaryExprNode {
    public:

        using UnaryExprNode::UnaryExprNode;


        void accept(NodeVisitor& visitor) const override;

        bool isNullable() const noexcept override {
            return true;
        }
    };

}
