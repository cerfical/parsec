#ifndef PARSEC_REGEX_BINARY_EXPR_HEADER
#define PARSEC_REGEX_BINARY_EXPR_HEADER

#include "ExprNode.hpp"

namespace parsec::regex {

	/**
	 * @brief Regular expression with two operands.
	 */
	class BinaryExpr : public ExprNode {
	public:

		BinaryExpr(ExprPtr lhs, ExprPtr rhs) noexcept
			: m_left(std::move(lhs)), m_right(std::move(rhs)) {
			m_left->setParent(this);
			
			m_right->rebaseAtomIndices(m_left->atomCount());
			m_right->setParent(this);
		}

		~BinaryExpr() override = default;


		int atomCount() const noexcept override {
			return m_left->atomCount() + m_right->atomCount();
		}


		/** @brief Left subexpression. */
		const ExprNode* left() const noexcept {
			return m_left.get();
		}
		
		/** @brief Right subexpression. */
		const ExprNode* right() const noexcept {
			return m_right.get();
		}


	private:
		void rebaseAtomIndices(int base) noexcept override {
			m_left->rebaseAtomIndices(base);
			m_right->rebaseAtomIndices(m_left->atomCount() + base);
		}

		ExprPtr m_left;
		ExprPtr m_right;
	};

}

#endif