#ifndef PARSEC_FG_ALTERN_NODE_HEADER
#define PARSEC_FG_ALTERN_NODE_HEADER

#include "BinaryNode.hpp"

namespace parsec::fg {
	/**
	 * @brief Alternation node.
	 */
	class AlternNode : public BinaryNode {
	public:
		/** @{ */
		/** @brief Construct a new node with two child nodes. */
		AlternNode(std::unique_ptr<SyntaxNode> left,std::unique_ptr<SyntaxNode> right) noexcept
			: BinaryNode(std::move(left), std::move(right))
		{ }
		/** @} */


		/** @{ */
		void traverseWith(NodeTraverser& traverser) const override;
		/** @} */
	};
}

#endif