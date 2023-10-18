#include "regex/UnaryExpr.hpp"

namespace parsec::regex {
	UnaryExpr::UnaryExpr(std::unique_ptr<ExprNode> inner) noexcept
		: m_inner(std::move(inner)) {
		m_inner->setParent(this);
	}
}