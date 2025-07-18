module;

#include <utility>

export module parsec.pars:ast.BinaryRuleNode;

import :ast.Node;

namespace parsec::pars {

    /**
     * @brief Rule consisting of two subrules.
     */
    export class BinaryRuleNode : public Node {
    public:

        BinaryRuleNode(NodePtr left, NodePtr right)
            : left_(std::move(left)), right_(std::move(right)) {}


        /**
         * @brief Left subrule.
         */
        const Node* left() const noexcept {
            return left_.get();
        }


        /**
         * @brief Right subrule.
         */
        const Node* right() const noexcept {
            return right_.get();
        }


    private:
        NodePtr left_;
        NodePtr right_;
    };

}
