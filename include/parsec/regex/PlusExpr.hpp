#ifndef PARSEC_REGEX_PLUS_EXPR_HEADER
#define PARSEC_REGEX_PLUS_EXPR_HEADER

#include "UnaryExpr.hpp"

namespace parsec::regex {
	/**
	 * @brief Kleene plus expression.
	 */
	class PlusExpr : public UnaryExpr {
	public:
		/** @{ */
		/** @brief Construct a new node with a single child node. */
		explicit PlusExpr(std::unique_ptr<ExprNode> inner) noexcept
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