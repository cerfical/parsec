#pragma once

#include "../utils/util_types.hpp"
#include "../hash.hpp"

#include "SymbolRule.hpp"

#include <unordered_map>
#include <vector>
#include <span>

namespace parsec {

	class SymbolGrammar : private NonCopyable {
	public:
		
		void define(const Symbol& symbol, const RegularExpr& expr);


		const SymbolRule& resolve(const Symbol& symbol) const;

		bool contains(const Symbol& symbol) const {
			syncRulesCache();
			return m_rulesCache.contains(symbol);
		}


		void setRoot(const Symbol& symbol) {
			m_rootSymbol = symbol;
		}

		const SymbolRule& root() const {
			return resolve(m_rootSymbol);
		}

		
		std::span<const SymbolRule> rules() const {
			return sortedRules();
		}

		std::span<const Symbol> symbols() const {
			return sortedSymbols();
		}


	private:
		void syncRulesCache() const;

		const std::vector<SymbolRule>& sortedRules() const;
		const std::vector<Symbol>& sortedSymbols() const;


		struct SymbolHasher {
			std::size_t operator()(const Symbol& symbol) const noexcept {
				return hash(symbol);
			}
		};

		mutable std::unordered_map<Symbol, std::size_t, SymbolHasher> m_rulesCache;

		mutable std::vector<Symbol> m_symbols;
		mutable bool m_symbolsSorted = true;
		
		mutable std::vector<SymbolRule> m_rules;
		mutable bool m_rulesSorted = true;


		Symbol m_rootSymbol;
	};

}
