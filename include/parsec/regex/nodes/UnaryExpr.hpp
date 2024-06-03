#pragma once

#include "ExprNode.hpp"

namespace parsec::regex {
	
	class UnaryExpr : public ExprNode {
	public:

		explicit UnaryExpr(NodePtr inner)
			: m_inner(inner) {}

		~UnaryExpr() override = default;


		int atomCount() const noexcept override {
			return m_inner->atomCount();
		}


		const ExprNode* inner() const {
			return m_inner.get();
		}


	private:
		NodePtr m_inner;
	};

}
