#ifndef PARSEC_REGEX_CONCAT_EXPR_HEADER
#define PARSEC_REGEX_CONCAT_EXPR_HEADER

#include "BinaryExpr.hpp"

namespace parsec::regex {
	class ConcatExpr : public BinaryExpr {
	public:
		/** @{ */
		ConcatExpr(ExprPtr left, ExprPtr right) noexcept
			: BinaryExpr(std::move(left), std::move(right))
		{ }
		/** @} */

		/** @{ */
		void acceptVisitor(NodeVisitor& visitor) const override;
		/** @} */
	};
}

#endif