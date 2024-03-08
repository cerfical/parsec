#ifndef PARSEC_FG_SYMBOL_GRAMMAR_HEADER
#define PARSEC_FG_SYMBOL_GRAMMAR_HEADER

#include "../core/NonCopyable.hpp"
#include "Rule.hpp"

#include <unordered_map>
#include <vector>
#include <span>

namespace parsec::fg {
	
	/**
	 * @brief Describes the syntax of a language using a list of inference rules.
	*/
	class SymbolGrammar : private NonCopyable {
	public:
		
		/**
		 * @brief Define a new symbol in the grammar.
		*/
		void insertSymbol(std::string_view symbol, Rule rule);



		/**
		 * @brief Find a rule that defines the specified symbol, if there is one.
		*/
		const Rule* resolveSymbol(std::string_view symbol) const;



		/**
		 * @brief Start symbol of the grammar, if there is one.
		*/
		std::string_view startSymbol() const {
			if(m_symbols.empty()) {
				return "";
			}
			return m_symbols.front();
		}



		/**
		 * @brief List of all defined symbols.
		*/
		std::span<const std::string_view> symbols() const {
			return m_symbols;
		}



	private:
		std::unordered_map<std::string, Rule> m_symbolRules;
		std::vector<std::string_view> m_symbols;
	};

}

#endif