#include "fg/SymbolGrammar.hpp"
#include <algorithm>

namespace parsec::fg {
	void SymbolGrammar::insertSymbol(std::string_view symbol, std::optional<Rule> rule) {
		if(symbol.empty()) {
			throw std::runtime_error("empty grammar symbol");
		}

		const auto [symbolRuleIt, wasInserted] = m_symbolRules.try_emplace(std::string(symbol), std::move(rule));
		if(!wasInserted) {
			throw std::runtime_error("duplicate grammar symbol");
		}

		try {
			m_sortedSymbols.emplace_back(symbolRuleIt->first);
		} catch(...) {
			m_symbolRules.erase(symbolRuleIt);
			throw;
		}
		m_symbolsSorted = false;
	}


	const SymbolGrammar::SymbolRule* SymbolGrammar::findNotNullRule(std::string_view symbol) const {
		const auto it = m_symbolRules.find(std::string(symbol));
		if(it != m_symbolRules.end() && it->second.has_value()) {
			return &(*it);
		}
		return nullptr;
	}


	std::span<const std::string_view> SymbolGrammar::symbols() const {
		if(!m_symbolsSorted) {
			std::ranges::sort(m_sortedSymbols);
			m_symbolsSorted = true;
		}
		return m_sortedSymbols;
	}
}