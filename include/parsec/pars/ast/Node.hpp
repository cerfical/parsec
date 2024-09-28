#pragma once

#include <concepts>
#include <memory>

namespace parsec::pars {

    class NodeVisitor;

    /**
     * @brief Polymorphic entry point into an AST node.
     */
    class Node {
    public:

        Node() = default;

        Node(const Node& other) = delete;
        Node& operator=(const Node& other) = delete;

        Node(Node&& other) = delete;
        Node& operator=(Node&& other) = delete;

        virtual ~Node() = default;


        /**
         * @brief Perform a "visit" operation on the node using a NodeVisitor instance.
         */
        virtual void accept(NodeVisitor& visitor) const = 0;
    };


    /**
     * @brief Owning pointer to a Node.
     */
    using NodePtr = std::unique_ptr<Node>;


    /**
     * @brief Create a Node of the specified type.
     */
    template <typename NodeType, typename... Args>
        requires std::derived_from<NodeType, Node>
    NodePtr makeNode(Args&&... args) {
        return std::make_unique<NodeType>(std::forward<Args>(args)...);
    }

}
