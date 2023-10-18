#ifndef PARSEC_FG_CONCAT_NODE_HEADER
#define PARSEC_FG_CONCAT_NODE_HEADER

#include "BinaryNode.hpp"

namespace parsec::fg {
	/**
	 * @brief Concatenation node.
	 */
	class ConcatNode : public BinaryNode {
	public:
		/** @{ */
		/** @brief Construct a new node with two child nodes. */
		ConcatNode(std::unique_ptr<SyntaxNode> left, std::unique_ptr<SyntaxNode> right) noexcept
			: BinaryNode(std::move(left), std::move(right))
		{ }
		/** @} */


		/** @{ */
		void traverseWith(NodeTraverser& traverser) const override;
		/** @} */
	};
}

#endif