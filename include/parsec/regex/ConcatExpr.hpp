#ifndef PARSEC_REGEX_CONCAT_EXPR_HEADER
#define PARSEC_REGEX_CONCAT_EXPR_HEADER

#include "BinaryExpr.hpp"

namespace parsec::regex {
	
	/**
	 * @brief Marks one expression as immediately following another.
	 */
	class ConcatExpr : public BinaryExpr {
	public:
		
		ConcatExpr(ExprPtr left, ExprPtr right) noexcept
			: BinaryExpr(std::move(left), std::move(right))
		{ }


		ConcatExpr(ConcatExpr&&) = default;
		ConcatExpr& operator=(ConcatExpr&&) = default;


		/** @{ */
		void acceptVisitor(NodeVisitor& visitor) const override;
		/** @} */

	};

}

#endif