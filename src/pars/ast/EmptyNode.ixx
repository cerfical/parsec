export module parsec.pars:ast.EmptyNode;

import :ast.Node;

namespace parsec::pars {

    /**
     * @brief Empty node.
     */
    export class EmptyNode : public Node {
    public:

        void accept(NodeVisitor& visitor) const override;
    };

}
