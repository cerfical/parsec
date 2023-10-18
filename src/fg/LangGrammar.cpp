#include "fg/LangGrammar.hpp"
#include <gsl/narrow>

namespace parsec::fg {
	void LangGrammar::addTokenRule(const std::string& name, std::unique_ptr<regex::ExprNode> pattern) {
		m_tokenRules.emplace_back(
			name,
			std::move(pattern),
			gsl::narrow_cast<int>(m_tokenRules.size())
		);
	}

	void LangGrammar::addSyntaxRule(const std::string& name, std::unique_ptr<GrammarNode> body) {
		m_syntaxRules.emplace_back(name, std::move(body));
	}
}