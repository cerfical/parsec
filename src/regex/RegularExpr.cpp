#include "regex/RegularExpr.hpp"

#include "regex/ast/PlusExpr.hpp"
#include "regex/ast/AlternExpr.hpp"
#include "regex/ast/ConcatExpr.hpp"
#include "regex/Parser.hpp"

#include <sstream>

namespace parsec::regex {
	RegularExpr::RegularExpr(std::string_view regex)
		: m_rootNode(Parser().parse(regex)) {
	}


	void RegularExpr::altern(RegularExpr other) {
		m_rootNode = ast::makeNode<ast::AlternExpr>(
			std::move(m_rootNode),
			std::move(other.m_rootNode)
		);
	}


	void RegularExpr::concat(RegularExpr other) {
		m_rootNode = ast::makeNode<ast::ConcatExpr>(
			std::move(m_rootNode),
			std::move(other.m_rootNode)
		);
	}


	void RegularExpr::repeat() {
		m_rootNode = ast::makeNode<ast::PlusExpr>(
			std::move(m_rootNode)
		);
	}


	std::string RegularExpr::toStr() const {
		std::ostringstream out;
		rootNode()->printTo(out);
		return out.str();
	}
}