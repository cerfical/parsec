#ifndef PARSEC_REGEX_OPTIONAL_EXPR_HEADER
#define PARSEC_REGEX_OPTIONAL_EXPR_HEADER

#include "UnaryExpr.hpp"

namespace parsec::regex {
	/**
	 * @brief Optional regular expression.
	 */
	class OptionalExpr : public UnaryExpr {
	public:
		/** @{ */
		/** @brief Construct a new node with a single child node. */
		explicit OptionalExpr(std::unique_ptr<ExprNode> inner) noexcept
			: UnaryExpr(std::move(inner))
		{ }
		/** @} */


		/** @{ */
		void traverseWith(NodeTraverser& traverser) const override;
		bool nullable() const noexcept override;
		/** @} */
	};
}

#endif