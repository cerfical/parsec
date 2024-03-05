#ifndef PARSEC_REGEX_AST_OPTIONAL_EXPR_HEADER
#define PARSEC_REGEX_AST_OPTIONAL_EXPR_HEADER

#include "UnaryExpr.hpp"

namespace parsec::regex::ast {
	
	/**
	 * @brief Marks an inner expression as 'optional', meaning it is either present or not.
	 */
	class OptionalExpr : public UnaryExpr {
	public:
		
		OptionalExpr() = default;

		explicit OptionalExpr(ExprPtr inner) noexcept
			: UnaryExpr(std::move(inner))
		{ }


		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif