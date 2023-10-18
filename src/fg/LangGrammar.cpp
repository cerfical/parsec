#include "fg/LangGrammar.hpp"

namespace parsec::fg {
	void LangGrammar::addTokenRule(const std::string& name, regex::RegEx pattern) {
		m_tokenRules.emplace_back(name, std::move(pattern), tokenRuleCount());
	}

	void LangGrammar::addSyntaxRule(const std::string& name, std::unique_ptr<GrammarNode> body) {
		m_syntaxRules.emplace_back(name, std::move(body));
	}
}