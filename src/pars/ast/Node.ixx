module;

#include <concepts>
#include <memory>

export module parsec.pars:ast.Node;

namespace parsec::pars {

    export class NodeVisitor;

    /**
     * @brief AST node to represent a grammar rule.
     */
    export class Node {
    public:

        Node() = default;

        Node(const Node& other) = delete;
        Node& operator=(const Node& other) = delete;

        Node(Node&& other) = delete;
        Node& operator=(Node&& other) = delete;

        virtual ~Node() = default;


        /** @{ */
        /**
         * @brief Perform a type-safe 'visit' on the node using a visitor.
         */
        virtual void accept(NodeVisitor& visitor) const = 0;
        /** @} */
    };


    /**
     * @brief Owning pointer to a Node.
     */
    export using NodePtr = std::unique_ptr<Node>;


    /**
     * @brief Create a Node of the specified type.
     */
    export template <typename NodeType, typename... Args>
        requires std::derived_from<NodeType, Node>
    NodePtr makeNode(Args&&... args) {
        return std::make_unique<NodeType>(std::forward<Args>(args)...);
    }

}
