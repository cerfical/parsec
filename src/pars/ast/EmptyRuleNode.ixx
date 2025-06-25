export module parsec.pars:ast.EmptyRuleNode;

import :ast.Node;

namespace parsec::pars {

    /**
     * @brief Rule matching empty string.
     */
    export class EmptyRuleNode : public Node {
    public:

        void accept(NodeVisitor& visitor) const override;
    };

}
