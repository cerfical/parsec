#ifndef PARSEC_BNF_GRAMMAR_HEADER
#define PARSEC_BNF_GRAMMAR_HEADER

#include "GrammarSymbol.hpp"

#include <gsl/narrow>

#include <unordered_map>
#include <deque>

namespace parsec::bnf {
	
	/**
	 * @brief Describes the syntax of a language using a GrammarRule tree hierarchy.
	*/
	class Grammar {
	public:
		
		Grammar() = default;

		Grammar(const Grammar&) = delete;
		Grammar& operator=(const Grammar&) = delete;

		Grammar(Grammar&&) = default;
		Grammar& operator=(Grammar&&) = default;



		/** @{ */
		/**
		 * @brief Create a unique grammar symbol for a name.
		*/
		GrammarSymbol* makeSymbol(std::string_view name);



		/**
		 * @brief Get the root symbol of the rule tree, if there is one.
		*/
		const GrammarSymbol* startSymbol() const {
			if(m_symbols.empty()) {
				return nullptr;
			}
			return &m_symbols.front();
		}
		/** @} */



	private:
		int uniqueSymbolId() const noexcept {
			return gsl::narrow_cast<int>(m_symbols.size());
		}

		std::unordered_map<std::string, GrammarSymbol*> m_nameToSymbol;
		std::deque<GrammarSymbol> m_symbols;
	};

}

#endif