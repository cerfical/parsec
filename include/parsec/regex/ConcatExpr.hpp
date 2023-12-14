#ifndef PARSEC_REGEX_CONCAT_EXPR_HEADER
#define PARSEC_REGEX_CONCAT_EXPR_HEADER

#include "BinaryExpr.hpp"

namespace parsec::regex {
	
	/**
	 * @brief Marks one expression as immediately following another.
	 */
	class ConcatExpr : public BinaryExpr {
	public:
		
		ConcatExpr(ExprPtr lhs, ExprPtr rhs) noexcept
			: BinaryExpr(std::move(lhs), std::move(rhs))
		{ }

		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif