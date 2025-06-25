module;

#include <utility>

export module parsec.pars:ast.UnaryRuleNode;

import :ast.Node;

namespace parsec::pars {

    /**
     * @brief Rule consisting of one subrule.
     */
    export class UnaryRuleNode : public Node {
    public:

        explicit UnaryRuleNode(NodePtr inner)
            : inner_(std::move(inner)) {}


        /**
         * @brief Inner subrule.
         */
        const Node* inner() const noexcept {
            return inner_.get();
        }


    private:
        NodePtr inner_;
    };

}
