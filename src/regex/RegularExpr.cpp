#include "regex/RegularExpr.hpp"
#include "regex/Parser.hpp"

namespace parsec::regex {
	RegularExpr::RegularExpr(std::string_view regex)
		: m_rootNode(Parser().parse(regex))
	{ }
}