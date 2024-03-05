#ifndef PARSEC_REGEX_NODES_NIL_EXPR_HEADER
#define PARSEC_REGEX_NODES_NIL_EXPR_HEADER

#include "ExprNode.hpp"

namespace parsec::regex::nodes {
	
	/**
	 * @brief Empty regular expression.
	*/
	class NilExpr : public ExprNode {
	public:
		
		void acceptVisitor(NodeVisitor& visitor) const override;

	};

}

#endif