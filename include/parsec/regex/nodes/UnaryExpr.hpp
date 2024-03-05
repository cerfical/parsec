#ifndef PARSEC_REGEX_NODES_UNARY_EXPR_HEADER
#define PARSEC_REGEX_NODES_UNARY_EXPR_HEADER

#include "ExprNode.hpp"

namespace parsec::regex::nodes {
	
	/**
	 * @brief Regular expression with one operand.
	*/
	class UnaryExpr : public ExprNode {
	public:

		UnaryExpr() = default;

		explicit UnaryExpr(ExprPtr inner) noexcept
			: inner(std::move(inner)) {
		}

		~UnaryExpr() override = default;



		/** @brief Inner subexpression. */
		ExprPtr inner;

	};

}

#endif