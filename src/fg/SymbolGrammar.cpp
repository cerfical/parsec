#include "fg/SymbolGrammar.hpp"
#include <algorithm>

namespace parsec::fg {
	void SymbolGrammar::define(const Symbol& symbol, const RegularExpr& expr) {
		if(!symbol) {
			throw std::runtime_error("empty grammar symbol");
		}
		syncRulesCache();

		const auto [nameToRuleIndexIt, wasInserted] = m_rulesCache.try_emplace(symbol, m_rules.size());
		if(!wasInserted) {
			throw std::runtime_error("duplicate grammar symbol");
		}

		try {
			m_symbols.push_back(symbol);
		} catch(...) {
			m_rulesCache.erase(nameToRuleIndexIt);
			throw;
		}

		try {
			m_rules.emplace_back(symbol, expr);
		} catch(...) {
			m_symbols.pop_back();
			m_rulesCache.erase(nameToRuleIndexIt);
			throw;
		}

		m_symbolsSorted = false;
		m_rulesSorted = false;
	}


	const SymbolRule& SymbolGrammar::resolve(const Symbol& symbol) const {
		syncRulesCache();

		const auto nameToRuleIndexIt = m_rulesCache.find(symbol);
		if(nameToRuleIndexIt == m_rulesCache.end()) {
			static SymbolRule emptyRule;
			return emptyRule;
		}
		return m_rules[nameToRuleIndexIt->second];
	}


	void SymbolGrammar::syncRulesCache() const {
		if(m_rulesCache.empty()) {
			for(std::size_t i = 0; i < m_rules.size(); i++) {
				try {
					m_rulesCache[m_rules[i].head()] = i;
				} catch(...) {
					m_rulesCache.clear();
					throw;
				}
			}
		}
	}


	const std::vector<Symbol>& SymbolGrammar::sortedSymbols() const {
		if(!m_symbolsSorted) {
			std::ranges::sort(m_symbols);
			m_symbolsSorted = true;
		}
		return m_symbols;
	}


	const std::vector<SymbolRule>& SymbolGrammar::sortedRules() const {
		if(!m_rulesSorted) {
			/*
			 * Sorting will likely change the order of rules in the vector, and so the cached indices will refer to rules with names
			 * other than the ones they originally referenced.
			 * Therefore, we clear the cache to force a recalculation, if it is needed later.
			*/
			m_rulesCache.clear();

			std::ranges::sort(m_rules, [](const auto& lhs, const auto& rhs) {
				return lhs.head() < rhs.head();
			});
			m_rulesSorted = true;
		}
		return m_rules;
	}
}