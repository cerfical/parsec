#ifndef PARSEC_REGEX_NODES_BINARY_EXPR_HEADER
#define PARSEC_REGEX_NODES_BINARY_EXPR_HEADER

#include "ExprNode.hpp"

namespace parsec::regex {

	class BinaryExpr : public ExprNode {
	public:

		BinaryExpr(NodePtr left, NodePtr right)
			: m_left(left), m_right(right) {}

		~BinaryExpr() override = default;


		int atomCount() const noexcept override {
			return m_left->atomCount() + m_right->atomCount();
		}


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