export module parsec.regex:ast.OptionalExprNode;

import :ast.UnaryExprNode;

namespace parsec::regex {

    /**
     * @brief Denotes an 'optional' expresion.
     */
    export class OptionalExprNode : public UnaryExprNode {
    public:

        using UnaryExprNode::UnaryExprNode;


        void accept(NodeVisitor& visitor) const override;

        bool isNullable() const noexcept override {
            return true;
        }
    };

}
