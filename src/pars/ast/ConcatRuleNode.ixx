export module parsec.pars:ast.ConcatRuleNode;

import :ast.BinaryRuleNode;

namespace parsec::pars {

    /**
     * @brief Concatenation of two rules.
     */
    export class ConcatRuleNode : public BinaryRuleNode {
    public:

        using BinaryRuleNode::BinaryRuleNode;

        void accept(NodeVisitor& visitor) const override;
    };

}
