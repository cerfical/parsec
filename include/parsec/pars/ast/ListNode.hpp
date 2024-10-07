#pragma once

#include "Node.hpp"

namespace parsec::pars {

    /**
     * @brief Sequence of two Node%s.
     */
    class ListNode : public Node {
    public:

        ListNode(NodePtr head, NodePtr tail)
            : head_(std::move(head)), tail_(std::move(tail)) {}

        void accept(NodeVisitor& visitor) const override;


        /**
         * @brief Head node of the list.
         */
        const Node* head() const noexcept {
            return head_.get();
        }


        /**
         * @brief Tail node of the list.
         */
        const Node* tail() const noexcept {
            return tail_.get();
        }


    private:
        NodePtr head_;
        NodePtr tail_;
    };

}
