#include "fg/Grammar.hpp"
#include <gsl/narrow>

namespace parsec::fg {
	void Grammar::addTokenRule(const std::string& name, std::unique_ptr<regex::ExprNode> pattern) {
		m_tokenRules.emplace_back(
			name,
			std::move(pattern),
			gsl::narrow_cast<int>(m_tokenRules.size())
		);
	}

	void Grammar::addSyntaxRule(const std::string& name, std::unique_ptr<SyntaxNode> body) {
		m_syntaxRules.emplace_back(name, std::move(body));
	}
}