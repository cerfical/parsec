#include "regex/regex.hpp"

namespace parsec::regex {
	std::ostream& operator<<(std::ostream& out, const RegEx& regex) {
		out << regex.toStr();
		return out;
	}


	RegEx::RegEx(std::unique_ptr<ExprNode> root) {
		m_rootNode = std::make_unique<ConcatExpr>(
			std::move(root), std::make_unique<CharLiteral>('#')
		);
	}

	RegEx::RegEx(std::string_view regex)
		: RegEx(Parser().parse(regex))
	{ }

	std::string RegEx::toStr() const {
		return static_cast<const BinaryExpr*>(rootNode())->leftSubexpr()->toStr();
	}
}