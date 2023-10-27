#include "fg/Grammar.hpp"
#include <gsl/narrow>

namespace parsec::fg {
	bool Grammar::addSymbol(const std::string& name, RulePtr rule, bool terminal) {
		// assign an unique identifier to the new symbol and try to insert it into the grammar
		const auto symbolId = gsl::narrow_cast<int>(m_symbols.size());
		const auto [it, wasEmplaced] = m_symbolIds.try_emplace(name, symbolId);
		
		if(wasEmplaced) {
			m_symbols.emplace_back(name,
				std::move(rule),
				terminal,
				symbolId
			);	
		}
		
		return wasEmplaced;
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