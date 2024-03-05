#ifndef PARSEC_REGEX_AST_CONCAT_EXPR_HEADER
#define PARSEC_REGEX_AST_CONCAT_EXPR_HEADER

#include "BinaryExpr.hpp"

namespace parsec::regex::ast {
	
	/**
	 * @brief Marks one expression as immediately following another.
	*/
	class ConcatExpr : public BinaryExpr {
	public:
		
		ConcatExpr() = default;

		ConcatExpr(ExprPtr left, ExprPtr right) noexcept
			: BinaryExpr(std::move(left), std::move(right))
		{ }


		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif