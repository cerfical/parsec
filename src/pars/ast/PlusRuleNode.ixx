export module parsec.pars:ast.PlusRuleNode;

import :ast.UnaryRuleNode;

namespace parsec::pars {

    /**
     * @brief Denotes an application of the Kleene plus operator to a rule.
     */
    export class PlusRuleNode : public UnaryRuleNode {
    public:

        using UnaryRuleNode::UnaryRuleNode;

        void accept(NodeVisitor& visitor) const override;
    };

}
