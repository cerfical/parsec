#ifndef PARSEC_FG_SYMBOL_GRAMMAR_HEADER
#define PARSEC_FG_SYMBOL_GRAMMAR_HEADER

#include "../core/NonCopyable.hpp"
#include "Rule.hpp"

#include <unordered_map>
#include <vector>
#include <span>
#include <optional>

namespace parsec::fg {
	
	/**
	 * @brief Describes the syntax of a language using a list of inference rules.
	*/
	class SymbolGrammar : private NonCopyable {
	public:
		
		/**
		 * @brief Define a new symbol in the grammar.
		*/
		void insertSymbol(std::string_view symbol, std::optional<Rule> rule = {});



		/**
		 * @brief Find a rule that defines the specified symbol, if there is one.
		*/
		const Rule* resolveSymbol(std::string_view symbol) const {
			if(const auto symbolRule = findNotNullRule(symbol)) {
				return &symbolRule->second.value();
			}
			return nullptr;
		}



		/**
		 * @brief Set the start symbol for the grammar.
		*/
		void setStartSymbol(std::string_view symbol) {
			if(const auto symbolRule = findNotNullRule(symbol)) {
				m_startSymbol = symbolRule->first;
			}
		}



		/**
		 * @brief Start symbol of the grammar, if there is one.
		*/
		std::string_view startSymbol() const {
			return m_startSymbol;
		}



		/**
		 * @brief List of all defined symbols.
		*/
		std::span<const std::string_view> symbols() const;



	private:
		using SymbolRuleTable = std::unordered_map<std::string, std::optional<Rule>>;
		using SymbolRule = SymbolRuleTable::value_type;


		const SymbolRule* findNotNullRule(std::string_view symbol) const;


		SymbolRuleTable m_symbolRules;
		std::string_view m_startSymbol;

		mutable std::vector<std::string_view> m_sortedSymbols;
		mutable bool m_symbolsSorted = true;
	};

}

#endif