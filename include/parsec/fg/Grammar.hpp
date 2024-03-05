#ifndef PARSEC_FG_GRAMMAR_HEADER
#define PARSEC_FG_GRAMMAR_HEADER

#include "../core/NonCopyable.hpp"
#include "GrammarSymbol.hpp"

#include <unordered_map>

namespace parsec::fg {
	
	/**
	 * @brief Describes the syntax of a language using a list of inference rules.
	*/
	class Grammar : private NonCopyable {
	public:
		
		/**
		 * @brief Allocate a new symbol in the grammar defined by a rule.
		*/
		void addSymbol(const std::string& name, RuleExpr rule);



		/**
		 * @brief Set a start symbol for the grammar.
		*/
		void markStartSymbol(const std::string& name) {
			m_startSymbol = lookupSymbol(name);
		}



		/**
		 * @brief Find a grammar symbol with the specified name, if there is one.
		*/
		const GrammarSymbol* lookupSymbol(const std::string& name) const;



		/**
		 * @brief Start symbol of the grammar, if there is one.
		*/
		const GrammarSymbol* startSymbol() const {
			return m_startSymbol;
		}



	private:
		Id getUniqueSymbolId() const noexcept {
			return m_symbols.size();
		}

		std::unordered_map<std::string, GrammarSymbol> m_symbols;
		const GrammarSymbol* m_startSymbol = {};
	};

}

#endif