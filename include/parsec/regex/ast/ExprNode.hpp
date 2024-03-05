#ifndef PARSEC_REGEX_AST_EXPR_NODE_HEADER
#define PARSEC_REGEX_AST_EXPR_NODE_HEADER

#include "../../core/NonMovable.hpp"
#include <ostream>
#include <memory>

namespace parsec::regex::ast {

	class NodeVisitor;

	/**
	 * @brief Abstract representation of a regular expression.
	*/
	class ExprNode : private NonMovable {
	public:

		virtual ~ExprNode() = default;



		/**
		 * @brief Perform a double-dispatch call to the specified visitor.
		*/
		virtual void acceptVisitor(NodeVisitor& visitor) const = 0;



		/**
		 * @brief Compare two nodes for equality.
		*/
		bool isEqualTo(const ExprNode& other) const noexcept;



		/**
		 * @brief Print out the node to an output stream.
		*/
		void printTo(std::ostream& out) const;

	};



	/**
	 * @brief Owning pointer to an ExprNode.
	*/
	using ExprPtr = std::unique_ptr<ExprNode>;



	/**
	 * @brief Create an ExprNode of the specified type.
	*/
	template <typename Node, typename... Args>
		requires std::derived_from<Node, ExprNode>
	ExprPtr makeNode(Args&&... args) {
		return std::make_unique<Node>(std::forward<Args>(args)...);
	}

}

#endif