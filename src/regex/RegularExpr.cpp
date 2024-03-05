#include "regex/RegularExpr.hpp"

#include "regex/nodes/PlusExpr.hpp"
#include "regex/nodes/AlternExpr.hpp"
#include "regex/nodes/ConcatExpr.hpp"
#include "regex/Parser.hpp"

#include <sstream>

namespace parsec::regex {
	RegularExpr::RegularExpr(std::string_view regex)
		: m_rootNode(Parser().parse(regex)) {
	}


	void RegularExpr::altern(RegularExpr other) {
		m_rootNode = nodes::makeExpr<nodes::AlternExpr>(
			std::move(m_rootNode),
			std::move(other.m_rootNode)
		);
	}


	void RegularExpr::concat(RegularExpr other) {
		m_rootNode = nodes::makeExpr<nodes::ConcatExpr>(
			std::move(m_rootNode),
			std::move(other.m_rootNode)
		);
	}


	void RegularExpr::repeat() {
		m_rootNode = nodes::makeExpr<nodes::PlusExpr>(
			std::move(m_rootNode)
		);
	}


	std::string RegularExpr::toStr() const {
		std::ostringstream out;
		rootNode()->printTo(out);
		return out.str();
	}
}