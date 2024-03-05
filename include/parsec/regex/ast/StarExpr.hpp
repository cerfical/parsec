#ifndef PARSEC_REGEX_AST_STAR_EXPR_HEADER
#define PARSEC_REGEX_AST_STAR_EXPR_HEADER

#include "UnaryExpr.hpp"

namespace parsec::regex::ast {
	
	/**
	 * @brief Marks an inner expression as being repeated zero or more times.
	*/
	class StarExpr : public UnaryExpr {
	public:
		
		StarExpr() = default;

		explicit StarExpr(ExprPtr inner) noexcept
			: UnaryExpr(std::move(inner))
		{ }


		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif