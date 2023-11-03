#include "fg/Grammar.hpp"
#include <gsl/narrow>

namespace parsec::fg {
	void Grammar::putSymbol(const std::string& name, RulePtr rule, SymbolTypes type) {
		// allocate a new unique symbol identifier if a symbol with the same name is not yet defined
		const auto newSymbolId = gsl::narrow_cast<int>(m_symbols.size());
		const auto [it, wasEmplaced] = m_symbolIds.try_emplace(name, newSymbolId);
		const auto oldSymbolId = it->second;

		// construct a new symbol in the proper location
		if(wasEmplaced) {
			m_symbols.emplace_back(name,
				std::move(rule),
				type,
				newSymbolId
			);	
		} else {
			m_symbols[oldSymbolId] = Symbol(name, std::move(rule), type, oldSymbolId);
		}
	}

	const Symbol* Grammar::lookupSymbol(const std::string& name) const {
		const auto it = m_symbolIds.find(name);
		if(it != m_symbolIds.cend()) {
			const auto id = it->second;
			return &m_symbols[id];
		}
		return nullptr;
	}
}