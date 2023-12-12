#ifndef PARSEC_REGEX_BINARY_EXPR_HEADER
#define PARSEC_REGEX_BINARY_EXPR_HEADER

#include "ExprNode.hpp"

namespace parsec::regex {

	/**
	 * Regular expression operation with two operands.
	 */
	class BinaryExpr : public ExprNode {
	public:

		BinaryExpr(ExprPtr left, ExprPtr right) noexcept
			: m_left(std::move(left)), m_right(std::move(right)) {
			m_left->setParent(this);
			m_right->setParent(this);
		}

		~BinaryExpr() override = default;


		const ExprNode* left() const noexcept {
			return m_left.get();
		}
		
		const ExprNode* right() const noexcept {
			return m_right.get();
		}


	private:
		ExprPtr m_left;
		ExprPtr m_right;
	};

}

#endif