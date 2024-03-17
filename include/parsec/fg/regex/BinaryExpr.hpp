#ifndef PARSEC_FG_REGEX_BINARY_EXPR_HEADER
#define PARSEC_FG_REGEX_BINARY_EXPR_HEADER

#include "ExprNode.hpp"

namespace parsec::fg::regex {

	class BinaryExpr : public ExprNode {
	public:

		BinaryExpr(NodePtr left, NodePtr right)
			: m_left(left), m_right(right) {}

		~BinaryExpr() override = default;


		const ExprNode* left() const {
			return m_left.get();
		}

		const ExprNode* right() const {
			return m_right.get();
		}


	private:
		NodePtr m_left;
		NodePtr m_right;
	};

}

#endif