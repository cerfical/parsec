#pragma once

#include "Node.hpp"

namespace parsec::pars {

    /**
     * @brief Rule consisting of two subrules.
     */
    class BinaryRule : public Node {
    public:

        BinaryRule(NodePtr left, NodePtr right) noexcept
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
