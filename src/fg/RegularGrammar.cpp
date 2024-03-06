#include "fg/RegularGrammar.hpp"

namespace parsec::fg {
	void RegularGrammar::addPattern(const std::string& name, regex::RegularExpr regex) {
		const auto [it, wasInserted] = m_patternIndex.try_emplace(name, getUniquePatternId());
		const auto& [patName, patId] = *it;
		
		if(!wasInserted) {
			throw std::runtime_error("duplicate regular pattern");
		}

		try {
			m_patterns.emplace_back(patName, std::move(regex), patId);
		} catch(...) {
			m_patternIndex.erase(it);
			throw;
		}
	}
}