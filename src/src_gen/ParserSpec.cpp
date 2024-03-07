#include "src_gen/ParserSpec.hpp"

namespace parsec::src_gen {
	void ParserSpec::defineRule(const std::string& name, fg::RuleExpr rule) {
		const auto [nameIt, wasInserted] = m_definedRules.insert(name);
		if(wasInserted) {
			try {
				m_ruleNames.emplace_back(*nameIt);
			} catch(...) {
				m_definedRules.erase(nameIt);
				throw;
			}
		}

		try {
			m_syntaxGrammar.insertSymbol(name, std::move(rule));
		} catch(...) {
			if(wasInserted) {
				m_ruleNames.pop_back();
				m_definedRules.erase(nameIt);
			}
			throw;
		}
	}

	bool ParserSpec::isRuleDefined(const std::string& name) const {
		return m_definedRules.contains(name);
	}
}