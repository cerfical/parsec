#ifndef PARSEC_REGEX_STAR_EXPR_HEADER
#define PARSEC_REGEX_STAR_EXPR_HEADER

#include "UnaryExpr.hpp"

namespace parsec::regex {
	/**
	 * @brief Kleene star expression.
	 */
	class StarExpr : public UnaryExpr {
	public:
		/** @{ */
		/** @brief Construct a new node with a single child node. */
		explicit StarExpr(std::unique_ptr<ExprNode> inner) noexcept
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