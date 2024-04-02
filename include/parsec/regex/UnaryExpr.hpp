#ifndef PARSEC_REGEX_UNARY_EXPR_HEADER
#define PARSEC_REGEX_UNARY_EXPR_HEADER

#include "ExprNode.hpp"

namespace parsec::regex {
	
	class UnaryExpr : public ExprNode {
	public:

		explicit UnaryExpr(NodePtr inner)
			: m_inner(inner) {}

		~UnaryExpr() override = default;


		const ExprNode* inner() const {
			return m_inner.get();
		}


	private:
		NodePtr m_inner;
	};

}

#endif