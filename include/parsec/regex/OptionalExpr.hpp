#ifndef PARSEC_REGEX_OPTIONAL_EXPR_HEADER
#define PARSEC_REGEX_OPTIONAL_EXPR_HEADER

#include "UnaryExpr.hpp"

namespace parsec::regex {
	/**
	 * @brief Optional expression node.
	 */
	class OptionalExpr : public UnaryExpr {
	public:
		explicit OptionalExpr(ExprPtr inner) noexcept
			: UnaryExpr(std::move(inner))
		{ }

		/** @{ */
		void acceptVisitor(NodeVisitor& visitor) const override;
		/** @} */
	};
}

#endif