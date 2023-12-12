#ifndef PARSEC_REGEX_PLUS_EXPR_HEADER
#define PARSEC_REGEX_PLUS_EXPR_HEADER

#include "UnaryExpr.hpp"

namespace parsec::regex {
	/**
	 * @brief Kleene star expression node.
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