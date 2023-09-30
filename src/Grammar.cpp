#include "Grammar.hpp"
#include "RegExParser.hpp"

namespace parsec {
	TokenPattern::TokenPattern(const std::string& name, const std::string& pattern, int priority)
	 : TokenPattern(name, RegExParser().Parse(pattern), priority)
	{ }

	TokenPattern::TokenPattern(const std::string& name, std::unique_ptr<RegExNode> regex, int priority)
	 : m_name(name) {
		m_regex = std::make_unique<RegExConcat>(
			std::move(regex), std::make_unique<RegExChar>('#')
		);
	}

	std::string TokenPattern::ToStr() const {
		if(m_regex) {
			return m_regex->ToStr();
		}
		return "";
	}
}