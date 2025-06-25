export module parsec.pars:ast.AlternRuleNode;

import :ast.BinaryRuleNode;

namespace parsec::pars {

    /**
     * @brief Alternation of two rules.
     */
    export class AlternRuleNode : public BinaryRuleNode {
    public:

        using BinaryRuleNode::BinaryRuleNode;

        void accept(NodeVisitor& visitor) const override;
    };

}
