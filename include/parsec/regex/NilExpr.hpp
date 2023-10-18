#ifndef PARSEC_REGEX_NIL_EXPR_HEADER
#define PARSEC_REGEX_NIL_EXPR_HEADER

#include "ExprNode.hpp"

namespace parsec::regex {
	/**
	 * @brief Empty regular expression.
	 */
	class NilExpr : public ExprNode {
	public:
		/** @{ */
		void traverseWith(NodeTraverser& traverser) const override;
		bool nullable() const noexcept override;
		/** @} */
	};
}

#endif