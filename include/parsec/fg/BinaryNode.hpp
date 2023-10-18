#ifndef PARSEC_FG_BINARY_NODE_HEADER
#define PARSEC_FG_BINARY_NODE_HEADER

#include "SyntaxNode.hpp"
#include <memory>

namespace parsec::fg {
	/**
	 * @brief Binary grammar node.
	 */
	class BinaryNode : public SyntaxNode {
	public:
		/** @{ */
		/** @brief Construct a new node with two child nodes. */
		BinaryNode(std::unique_ptr<SyntaxNode> left, std::unique_ptr<SyntaxNode> right) noexcept
			: m_left(std::move(left)), m_right(std::move(right))
		{ }
		
		~BinaryNode() override = default;
		/** @} */


		/** @{ */
		/** @brief Left child of the node. */
		const SyntaxNode* left() const noexcept {
			return m_left.get();
		}


		/** @brief Right child of the node. */
		const SyntaxNode* right() const noexcept {
			return m_right.get();
		}
		/** @} */


	private:
		std::unique_ptr<SyntaxNode> m_left;
		std::unique_ptr<SyntaxNode> m_right;
	};
}

#endif