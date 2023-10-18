#ifndef PARSEC_REGEX_UNARY_EXPR_HEADER
#define PARSEC_REGEX_UNARY_EXPR_HEADER

#include "ExprNode.hpp"
#include <memory>

namespace parsec::regex {
	/**
	 * @brief Unary regex operator.
	 */
	class UnaryExpr : public ExprNode {
	public:
		/** @{ */
		/** @brief Construct a new node with a single child node. */
		explicit UnaryExpr(std::unique_ptr<ExprNode> inner) noexcept
			: m_inner(std::move(inner)) {
			m_inner->setParent(this);
		}

		~UnaryExpr() override = default;
		/** @} */


		/** @{ */
		/** @brief Inner child of the node. */
		const ExprNode* inner() const noexcept {
			return m_inner.get();
		}
		/** @} */


	private:
		/** @{ */
		std::unique_ptr<ExprNode> m_inner;
		/** @} */
	};
}

#endif