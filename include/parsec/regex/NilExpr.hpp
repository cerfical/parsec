#ifndef PARSEC_REGEX_NIL_EXPR_HEADER
#define PARSEC_REGEX_NIL_EXPR_HEADER

#include "ExprNode.hpp"

namespace parsec::regex {
	/**
	 * @brief Empty expression node.
	 */
	class NilExpr : public ExprNode {
	public:
		void acceptVisitor(NodeVisitor& visitor) const override;
	};
}

#endif