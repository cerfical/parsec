#include "fg/RegularGrammar.hpp"

namespace parsec::fg {
	void RegularGrammar::addPattern(std::string name, regex::RegularExpr regex) {
		const auto [it, wasInserted] = m_patternIndex.try_emplace(std::move(name), getUniquePatternId());
		const auto& [patName, patId] = *it;

		if(wasInserted) {
			try {
				m_patterns.emplace_back(patName, std::move(regex), patId);
			} catch(...) {
				m_patternIndex.erase(it);
				throw;
			}
		} else {
			m_patterns[patId].addExpr(std::move(regex));
		}
	}
}