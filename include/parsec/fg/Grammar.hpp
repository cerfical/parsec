#ifndef PARSEC_FG_GRAMMAR_HEADER
#define PARSEC_FG_GRAMMAR_HEADER

#include "Symbol.hpp"
#include <unordered_map>

namespace parsec::fg {
	class Grammar {
	public:
		/** @{ */
		Grammar() = default;
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
		void putSymbol(const std::string& name, RulePtr rule, bool terminal = true);
		
		const Symbol* lookupSymbol(const std::string& name) const;

		const SymbolList& symbols() const noexcept {
			return m_symbols;
		}
		/** @} */


	private:
		/** @{ */
		std::unordered_map<std::string, int> m_symbolIds;
		SymbolList m_symbols;
		/** @} */
	};
}

#endif