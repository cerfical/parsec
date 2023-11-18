#ifndef PARSEC_REGEX_STAR_EXPR_HEADER
#define PARSEC_REGEX_STAR_EXPR_HEADER

#include "UnaryExpr.hpp"

namespace parsec::regex {
	class StarExpr : public UnaryExpr {
	public:
		/** @{ */
		explicit StarExpr(ExprPtr inner) noexcept
			: UnaryExpr(std::move(inner))
		{ }
		/** @} */

		/** @{ */
		void acceptVisitor(NodeVisitor& visitor) const override;
		/** @} */
	};
}

#endif