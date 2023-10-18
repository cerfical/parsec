#ifndef PARSEC_REGEX_BINARY_EXPR_HEADER
#define PARSEC_REGEX_BINARY_EXPR_HEADER

#include "ExprNode.hpp"
#include <memory>

namespace parsec::regex {
	/**
	 * @brief Binary regex operator.
	 */
	class BinaryExpr : public ExprNode {
	public:
		/** @{ */
		/** @brief Construct a new node with two child nodes. */
		BinaryExpr(std::unique_ptr<ExprNode> left, std::unique_ptr<ExprNode> right) noexcept;

		~BinaryExpr() override = default;
		/** @} */


		/** @{ */
		/** @brief Left child of the node. */
		const ExprNode* left() const noexcept {
			return m_left.get();
		}


		/** @brief Right child of the node. */
		const ExprNode* right() const noexcept {
			return m_right.get();
		}
		/** @} */


	private:
		/** @{ */
		std::unique_ptr<ExprNode> m_left;
		std::unique_ptr<ExprNode> m_right;
		/** @} */
	};
}

#endif