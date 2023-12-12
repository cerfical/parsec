#ifndef PARSEC_REGEX_OPTIONAL_EXPR_HEADER
#define PARSEC_REGEX_OPTIONAL_EXPR_HEADER

#include "UnaryExpr.hpp"

namespace parsec::regex {
	
	/**
	 * Marks an inner expression as 'optional', meaning it is either present or not.
	 */
	class OptionalExpr : public UnaryExpr {
	public:
		
		explicit OptionalExpr(ExprPtr inner) noexcept
			: UnaryExpr(std::move(inner))
		{ }


		OptionalExpr(OptionalExpr&&) = default;
		OptionalExpr& operator=(OptionalExpr&&) = default;


		/** @{ */
		void acceptVisitor(NodeVisitor& visitor) const override;
		/** @} */

	};

}

#endif