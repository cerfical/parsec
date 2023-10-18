#include "fg/TokenRule.hpp"
#include "regex/ConcatExpr.hpp"

namespace parsec::fg {
	std::ostream& operator<<(std::ostream& out, const TokenRule& r) {
		out << r.name() << " = \"" << *r.pattern() << '"';
		return out;
	}


	TokenRule::TokenRule(const std::string& name, std::unique_ptr<regex::ExprNode> pattern, int priority)
		: m_name(name), m_priority(priority) {
		m_pattern = std::make_unique<regex::ConcatExpr>(
			std::move(pattern), std::make_unique<regex::CharLiteral>('#')
		);
	}
}