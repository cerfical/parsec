#include "fg/SymbolGrammar.hpp"
#include <algorithm>

namespace parsec::fg {
	void SymbolGrammar::insertSymbol(std::string_view symbol, Rule rule) {
		if(symbol.empty()) {
			throw std::runtime_error("empty grammar symbol");
		}

		const auto [symbolRuleIt, wasInserted] = m_rules.try_emplace(std::string(symbol), std::move(rule));
		if(!wasInserted) {
			throw std::runtime_error("duplicate grammar symbol");
		}

		try {
			m_symbols.emplace_back(symbolRuleIt->first);
		} catch(...) {
			m_rules.erase(symbolRuleIt);
			throw;
		}
		m_symbolsSorted = false;
	}


	const Rule* SymbolGrammar::resolveSymbol(std::string_view name) const {
		const auto it = m_rules.find(std::string(name));
		if(it != m_rules.end()) {
			return &it->second;
		}
		return nullptr;
	}


	std::span<const std::string_view> SymbolGrammar::symbols() const {
		if(!m_symbolsSorted) {
			std::ranges::sort(m_symbols);
			m_symbolsSorted = true;
		}
		return m_symbols;
	}
}