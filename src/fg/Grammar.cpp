#include "fg/Grammar.hpp"

namespace parsec::fg {
	void Grammar::addSymbol(const std::string& name, RuleExpr rule) {
		const auto [it, wasInserted] = m_symbols.try_emplace(name, name, std::move(rule), getUniqueSymbolId());
		if(!wasInserted) {
			throw std::runtime_error("duplicate grammar symbol");
		}

		// make the first added symbol the start symbol
		if(!m_startSymbol) {
			m_startSymbol = &it->second;
		}
	}

	const GrammarSymbol* Grammar::lookupSymbol(const std::string& name) const {
		const auto it = m_symbols.find(name);
		if(it != m_symbols.end()) {
			return &it->second;
		}
		return nullptr;
	}
}