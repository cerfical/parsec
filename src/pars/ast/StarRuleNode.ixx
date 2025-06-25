export module parsec.pars:ast.StarRuleNode;

import :ast.UnaryRuleNode;

namespace parsec::pars {

    /**
     * @brief Denotes an application of the Kleene star operator to a rule.
     */
    export class StarRuleNode : public UnaryRuleNode {
    public:

        using UnaryRuleNode::UnaryRuleNode;

        void accept(NodeVisitor& visitor) const override;
    };

}
