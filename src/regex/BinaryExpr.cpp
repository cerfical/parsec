#include "regex/BinaryExpr.hpp"

namespace parsec::regex {
	BinaryExpr::BinaryExpr(
		std::unique_ptr<ExprNode> left,
		std::unique_ptr<ExprNode> right
	) noexcept
		: m_left(std::move(left)), m_right(std::move(right)) {
		m_left->setParent(this);
		m_right->setParent(this);
	}
}