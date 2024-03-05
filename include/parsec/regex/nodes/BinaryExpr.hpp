#ifndef PARSEC_REGEX_NODES_BINARY_EXPR_HEADER
#define PARSEC_REGEX_NODES_BINARY_EXPR_HEADER

#include "ExprNode.hpp"

namespace parsec::regex::nodes {

	/**
	 * @brief Regular expression with two operands.
	*/
	class BinaryExpr : public ExprNode {
	public:

		BinaryExpr() = default;

		BinaryExpr(ExprPtr left, ExprPtr right) noexcept
			: left(std::move(left)), right(std::move(right)) {
		}

		~BinaryExpr() override = default;



		/** @brief Left subexpression. */
		ExprPtr left;
		
		/** @brief Right subexpression. */
		ExprPtr right;

	};

}

#endif