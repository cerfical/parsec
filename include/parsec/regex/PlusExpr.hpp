#ifndef PARSEC_REGEX_PLUS_EXPR_HEADER
#define PARSEC_REGEX_PLUS_EXPR_HEADER

#include "UnaryExpr.hpp"

namespace parsec::regex {

	/**
	 * @brief Marks an inner expression as being repeated one or more times.
	 */
	class PlusExpr : public UnaryExpr {
	public:

		explicit PlusExpr(ExprPtr inner) noexcept
			: UnaryExpr(std::move(inner))
		{ }

		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif