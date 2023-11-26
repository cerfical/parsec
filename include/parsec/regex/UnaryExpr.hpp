#ifndef PARSEC_REGEX_UNARY_EXPR_HEADER
#define PARSEC_REGEX_UNARY_EXPR_HEADER

#include "ExprNode.hpp"

namespace parsec::regex {
	/**
	 * @brief Unary expression node.
	 */
	class UnaryExpr : public ExprNode {
	public:
		explicit UnaryExpr(ExprPtr inner) noexcept
			: m_inner(std::move(inner)) {
			m_inner->setParent(this);
		}

		~UnaryExpr() override = default;


		/** @{ */
		const ExprNode* inner() const noexcept {
			return m_inner.get();
		}
		/** @} */

	private:
		ExprPtr m_inner;
	};
}

#endif