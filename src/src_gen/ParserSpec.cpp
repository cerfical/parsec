#include "src_gen/ParserSpec.hpp"
#include "src_gen/utils.hpp"

namespace parsec::src_gen {
	void ParserSpec::defineRule(const std::string& name, fg::RuleExpr rule) {
		const auto [it, wasInserted] = m_definedRules.insert(utils::normalizeName(name));
		const auto& normalizedName = *it;

		if(wasInserted) {
			try {
				m_ruleNames.emplace_back(normalizedName);
			} catch(...) {
				m_definedRules.erase(it);
				throw;
			}
		}

		try {
			m_syntaxGrammar.addSymbol(name, std::move(rule));
		} catch(...) {
			if(wasInserted) {
				m_ruleNames.pop_back();
				m_definedRules.erase(it);
			}
			throw;
		}
	}

	bool ParserSpec::isRuleDefined(const std::string& name) const {
		return m_definedRules.contains(utils::normalizeName(name));
	}
}