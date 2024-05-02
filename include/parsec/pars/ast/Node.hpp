#ifndef PARSEC_PARS_AST_NODE_HEADER
#define PARSEC_PARS_AST_NODE_HEADER

#include "../utils/util_types.hpp"

#include <concepts>
#include <memory>

namespace parsec::pars {

	class NodeVisitor;

	/**
	 * @brief Polymorphic entry point into an AST node.
	*/
	class Node : private NonCopyable, private NonMovable {
	public:

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

#endif