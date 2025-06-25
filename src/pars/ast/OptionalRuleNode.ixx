export module parsec.pars:ast.OptionalRuleNode;

import :ast.UnaryRuleNode;

namespace parsec::pars {

    /**
     * @brief Denotes an 'optional' rule.
     */
    export class OptionalRuleNode : public UnaryRuleNode {
    public:

        using UnaryRuleNode::UnaryRuleNode;

        void accept(NodeVisitor& visitor) const override;
    };

}
