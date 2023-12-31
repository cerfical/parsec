#ifndef PARSEC_REGEX_ALTERN_EXPR_HEADER
#define PARSEC_REGEX_ALTERN_EXPR_HEADER

#include "BinaryExpr.hpp"

namespace parsec::regex {
	/**
	 * @brief Alternation expression node.
	 */
	class AlternExpr : public BinaryExpr {
	public:
		AlternExpr(ExprPtr left, ExprPtr right) noexcept
			: BinaryExpr(std::move(left), std::move(right))
		{ }

		/** @{ */
		void acceptVisitor(NodeVisitor& visitor) const override;
		/** @} */
	};
}

#endif