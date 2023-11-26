#include "core/Grammar.hpp"
#include <gsl/narrow>

namespace parsec {
	const Symbol* Grammar::symbolForName(const std::string& name) const {
		const auto [it, wasInserted] = m_symbols.try_emplace(name,
			name, gsl::narrow_cast<int>(m_symbols.size())
		);
		
		auto symbol = &it->second;
		if(wasInserted) {
			m_sortedSymbols.push_back(symbol);
		}
		return symbol;
	}


	Pattern* Grammar::insertPattern(const Symbol* head) {
		auto h = m_sortedSymbols[head->id()];
		if(!h->pattern()) {
			auto pattern = &m_tokenPatterns.emplace_back(
				head, gsl::narrow_cast<int>(m_tokenPatterns.size())
			);
			h->setPattern(pattern);
		}

		return h->pattern();
	}

	Rule* Grammar::insertRule(const Symbol* head) {
		auto h = m_sortedSymbols[head->id()];
		auto rule = &m_rules.emplace_back(
			h, gsl::narrow_cast<int>(m_rules.size())
		);

		h->addRule(rule);
		return rule;
	}
	
	
	Grammar::Grammar() {
		// preallocate memory for special symbols
		symbolForName("Root");
		symbolForName("Eof");
		symbolForName("Ws");
	}
}