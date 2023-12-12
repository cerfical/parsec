#ifndef PARSEC_REGEX_ALTERN_EXPR_HEADER
#define PARSEC_REGEX_ALTERN_EXPR_HEADER

#include "BinaryExpr.hpp"

namespace parsec::regex {

	/**
	 * @brief Marks two expressions as 'alternatives' to each other.
	 */
	class AlternExpr : public BinaryExpr {
	public:
		
		AlternExpr(ExprPtr left, ExprPtr right) noexcept
			: BinaryExpr(std::move(left), std::move(right))
		{ }


		AlternExpr(AlternExpr&&) = default;
		AlternExpr& operator=(AlternExpr&&) = default;


		/** @{ */
		void acceptVisitor(NodeVisitor& visitor) const override;
		/** @} */

	};

}

#endif