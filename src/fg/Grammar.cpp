#include "fg/Grammar.hpp"

#include <gsl/narrow>
#include <stdexcept>

namespace parsec::fg {
	void Grammar::addTokenRule(const std::string& name, std::unique_ptr<regex::ExprNode> pattern) {
		const auto it = m_rules.try_emplace(name, m_tokens.size(), true);
		if(!it.second) {
			throw std::logic_error("duplicate grammar rule name");
		}
		
		m_tokens.emplace_back(
			name,
			std::move(pattern),
			gsl::narrow_cast<int>(m_tokens.size())
		);
	}

	void Grammar::addSyntaxRule(const std::string& name, std::unique_ptr<rules::Rule> body) {
		const auto it = m_rules.try_emplace(name, m_syntax.size(), false);
		if(!it.second) {
			throw std::logic_error("duplicate grammar rule name");
		}

		m_syntax.emplace_back(name, std::move(body));
	}


	const GrammarRule* Grammar::resolveRule(const std::string& ruleName) const {
		const auto it = m_rules.find(ruleName);
		if(it != m_rules.cend()) {
			if(it->second.second) {
				return &m_tokens[it->second.first];
			} else {
				return &m_syntax[it->second.first];
			}
		}
		return nullptr;
	}
}