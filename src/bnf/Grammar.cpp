#include "bnf/Grammar.hpp"

namespace parsec::bnf {
	GrammarSymbol* Grammar::makeSymbol(std::string_view name) {
		const auto [it, inserted] = m_nameToSymbol.try_emplace(std::string(name), nullptr);
		auto& symbol = it->second;
		
		if(inserted) {
			m_symbols.emplace_back(name, uniqueSymbolId());
			symbol = &m_symbols.back();
		}
		return symbol;
	}
}