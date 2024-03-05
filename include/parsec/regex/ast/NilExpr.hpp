#ifndef PARSEC_REGEX_AST_NIL_EXPR_HEADER
#define PARSEC_REGEX_AST_NIL_EXPR_HEADER

#include "ExprNode.hpp"

namespace parsec::regex::ast {
	
	/**
	 * @brief Empty regular expression.
	*/
	class NilExpr : public ExprNode {
	public:
		
		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif