#ifndef PARSEC_REGEX_AST_ALTERN_EXPR_HEADER
#define PARSEC_REGEX_AST_ALTERN_EXPR_HEADER

#include "BinaryExpr.hpp"

namespace parsec::regex::ast {

	/**
	 * @brief Marks two expressions as 'alternatives' to each other.
	*/
	class AlternExpr : public BinaryExpr {
	public:
		
		AlternExpr(ExprPtr left, ExprPtr right) noexcept
			: BinaryExpr(std::move(left), std::move(right))
		{ }

		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif