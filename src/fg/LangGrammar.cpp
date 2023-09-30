#include "fg/LangGrammar.hpp"

namespace parsec::fg {
	std::ostream& operator<<(std::ostream& out, const GrammarRule& r) {
		out << r.toStr();
		return out;
	}


	std::string TokenRule::toStr() const {
		return name() + " = \"" + m_pattern.toStr() + '\"';
	}


	void LangGrammar::addTokenRule(const std::string& name, regex::RegEx pattern) {
		m_tokenRules.emplace_back(name, std::move(pattern), tokenRuleCount());
	}
}