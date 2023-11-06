#ifndef PARSEC_FG_GRAMMAR_HEADER
#define PARSEC_FG_GRAMMAR_HEADER

#include "Symbol.hpp"

#include <unordered_map>
#include <optional>

namespace parsec::fg {
	class Grammar {
	public:
		/** @{ */
		Grammar();
		/** @} */


		/** @{ */
		Grammar(Grammar&&) = default;
		Grammar& operator=(Grammar&&) = default;
		/** @} */

		/** @{ */
		Grammar(const Grammar&) = delete;
		Grammar& operator=(const Grammar&) = delete;
		/** @} */


		/** @{ */
		const Symbol* addTerminal(const std::string& name, RulePtr rule) {
			return addSymbol(name, std::move(rule), SymbolTypes::Terminal);
		}

		const Symbol* addNonterminal(const std::string& name, RulePtr rule) {
			return addSymbol(name, std::move(rule), SymbolTypes::Nonterminal);
		}
		/** @} */


		/** @{ */
		const Symbol* symbolByName(const std::string& name) const;

		const Symbol* symbolById(int id) const noexcept {
			if(id < m_symbols.size()) {
				return m_symbols[id];
			}
			return nullptr;
		}
		/** @} */


		/** @{ */
		const SymbolList& terminals() const noexcept {
			return m_terminals;
		}

		const SymbolList& nonterminals() const noexcept {
			return m_nonterminals;
		}
		
		const SymbolList& symbols() const noexcept {
			return m_symbols;
		}

		const Symbol* startSymbol() const;

		const Symbol* eofSymbol() const {
			return symbolById(0);
		}
		/** @} */


	private:
		Symbol* addSymbol(const std::string& name, RulePtr rule, SymbolTypes type);


		mutable std::optional<Symbol> m_startSymbol;

		std::unordered_map<std::string, Symbol> m_symbolTable;
		SymbolList m_nonterminals;
		SymbolList m_terminals;
		SymbolList m_symbols;
	};
}

#endif