#ifndef PARSEC_FG_SYMBOL_GRAMMAR_HEADER
#define PARSEC_FG_SYMBOL_GRAMMAR_HEADER

#include "../core/NonCopyable.hpp"
#include "RuleExpr.hpp"

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
		 * @brief Allocate a new symbol in the grammar defined by a rule.
		*/
		void insertSymbol(std::string_view symbol, RuleExpr rule);



		/**
		 * @brief Find a rule that defines the specified symbol, if there is one.
		*/
		const RuleExpr* resolveSymbol(std::string_view symbol) const;



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
		 * @brief List of all defined symbols in sorted lexicographical order.
		*/
		std::span<const std::string_view> symbols() const;



	private:
		std::unordered_map<std::string, RuleExpr> m_rules;
		
		mutable std::vector<std::string_view> m_symbols;
		mutable bool m_symbolsSorted = true;
	};

}

#endif