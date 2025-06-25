export module parsec.regex:ast.AlternExprNode;

import :ast.BinaryExprNode;

namespace parsec::regex {

    /**
     * @brief Alternation of two expressions.
     */
    export class AlternExprNode : public BinaryExprNode {
    public:

        using BinaryExprNode::BinaryExprNode;


        void accept(NodeVisitor& visitor) const override;

        bool isNullable() const noexcept override {
            return left()->isNullable() || right()->isNullable();
        }
    };

}
