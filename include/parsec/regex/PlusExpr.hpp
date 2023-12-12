#ifndef PARSEC_REGEX_PLUS_EXPR_HEADER
#define PARSEC_REGEX_PLUS_EXPR_HEADER

#include "UnaryExpr.hpp"

namespace parsec::regex {

	/**
	 * Marks an inner expression as being repeated one or more times.
	 */
	class PlusExpr : public UnaryExpr {
	public:

		explicit PlusExpr(ExprPtr inner) noexcept
			: UnaryExpr(std::move(inner))
		{ }


		PlusExpr(PlusExpr&&) = default;
		PlusExpr& operator=(PlusExpr&&) = default;


		/** @{ */
		void acceptVisitor(NodeVisitor& visitor) const override;
		/** @} */

	};

}

#endif