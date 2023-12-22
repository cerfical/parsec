#ifndef PARSEC_REGEX_NODES_ALTERN_EXPR_HEADER
#define PARSEC_REGEX_NODES_ALTERN_EXPR_HEADER

#include "BinaryExpr.hpp"

namespace parsec::regex::nodes {

	/**
	 * @brief Marks two expressions as 'alternatives' to each other.
	 */
	class AlternExpr : public BinaryExpr {
	public:
		
		AlternExpr(ExprPtr lhs, ExprPtr rhs) noexcept
			: BinaryExpr(std::move(lhs), std::move(rhs))
		{ }

		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif