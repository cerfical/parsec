#ifndef PARSEC_REGEX_CONCAT_EXPR_HEADER
#define PARSEC_REGEX_CONCAT_EXPR_HEADER

#include "BinaryExpr.hpp"

namespace parsec::regex {
	/**
	 * @brief Concatenation expression.
	 */
	class ConcatExpr : public BinaryExpr {
	public:
		/** @{ */
		/** @brief Construct a new node with two child nodes. */
		ConcatExpr(std::unique_ptr<ExprNode> left, std::unique_ptr<ExprNode> right) noexcept
			: BinaryExpr(std::move(left), std::move(right))
		{ }
		/** @} */


		/** @{ */
		void traverseWith(NodeTraverser& traverser) const override;
		bool nullable() const noexcept override;
		/** @} */
	};
}

#endif