#ifndef PARSEC_REGEX_UNARY_EXPR_HEADER
#define PARSEC_REGEX_UNARY_EXPR_HEADER

#include "ExprNode.hpp"

namespace parsec::regex {
	
	/**
	 * @brief Regular expression with one operand.
	 */
	class UnaryExpr : public ExprNode {
	public:

		explicit UnaryExpr(ExprPtr inner) noexcept {
			inner->setParent(this);
			m_inner = std::move(inner);
		}

		~UnaryExpr() override = default;


		int atomCount() const noexcept override {
			return m_inner->atomCount();
		}


		/** @brief Inner subexpression. */
		const ExprNode* inner() const noexcept {
			return m_inner.get();
		}


	private:
		void rebaseAtomIndices(int base) noexcept override {
			m_inner->rebaseAtomIndices(base);
		}

		ExprPtr m_inner;
	};

}

#endif