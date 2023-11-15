#ifndef PARSEC_FG_GRAMMAR_HEADER
#define PARSEC_FG_GRAMMAR_HEADER

#include "Symbol.hpp"

#include <unordered_map>
#include <gsl/narrow>

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
		const Symbol* insertToken(const std::string& name, RulePtr rule) {
			return insertSymbol(name, std::move(rule), SymbolTypes::Token);
		}

		const Symbol* insertRule(const std::string& name, RulePtr rule) {
			return insertSymbol(name, std::move(rule), SymbolTypes::Rule);
		}
		/** @} */


		/** @{ */
		const Symbol* symbolByName(const std::string& name) const;


		const SymbolList& tokens() const noexcept {
			return m_tokens;
		}

		int tokenCount() const noexcept {
			return gsl::narrow_cast<int>(m_tokens.size());
		}

		bool anyTokens() const noexcept {
			return !m_tokens.empty();
		}


		const SymbolList& rules() const noexcept {
			return m_rules;
		}
		
		int ruleCount() const noexcept {
			return gsl::narrow_cast<int>(m_rules.size());
		}

		bool anyRules() const noexcept {
			return !m_rules.empty();
		}


		const SymbolList& symbols() const noexcept {
			startSymbol();
			return m_symbols;
		}

		int symbolCount() const noexcept {
			return gsl::narrow_cast<int>(m_symbols.size());
		}

		bool anySymbols() const noexcept {
			return !m_symbols.empty();
		}
		/** @} */


		/** @{ */
		const Symbol* startSymbol() const;

		const Symbol* eofToken() const {
			return symbols()[1];
		}

		const Symbol* wsToken() const {
			return symbols()[2];
		}
		/** @} */


	private:
		Symbol* insertSymbol(const std::string& name, RulePtr rule, SymbolTypes type);
		
		void initNewSymbol(Symbol& sym, RulePtr rule);
		void appendRuleToSymbol(Symbol& sym, RulePtr rule) const;
		

		std::unordered_map<std::string, Symbol> m_symbolTable;
		
		SymbolList m_symbols;
		SymbolList m_tokens;
		SymbolList m_rules;
	};
}

#endif