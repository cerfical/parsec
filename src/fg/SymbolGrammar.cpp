#include "fg/SymbolGrammar.hpp"

namespace parsec::fg {
	void SymbolGrammar::insertSymbol(std::string_view symbol, Rule rule) {
		if(symbol.empty()) {
			throw std::runtime_error("empty grammar symbol");
		}

		const auto [symbolRuleIt, wasInserted] = m_symbolRules.try_emplace(std::string(symbol), std::move(rule));
		if(!wasInserted) {
			throw std::runtime_error("duplicate grammar symbol");
		}

		try {
			m_symbols.emplace_back(symbolRuleIt->first);
		} catch(...) {
			m_symbolRules.erase(symbolRuleIt);
			throw;
		}
	}


	const Rule* SymbolGrammar::resolveSymbol(std::string_view symbol) const {
		const auto it = m_symbolRules.find(std::string(symbol));
		if(it != m_symbolRules.end()) {
			return &it->second;
		}
		return nullptr;
	}
}