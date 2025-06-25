export module parsec.regex:ast.ConcatExprNode;

import :ast.BinaryExprNode;

namespace parsec::regex {

    /**
     * @brief Concatenation of two expressions.
     */
    export class ConcatExprNode : public BinaryExprNode {
    public:

        using BinaryExprNode::BinaryExprNode;


        void accept(NodeVisitor& visitor) const override;

        bool isNullable() const noexcept override {
            return left()->isNullable() && right()->isNullable();
        }
    };

}
