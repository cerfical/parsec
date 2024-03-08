#include "src_gen/ParserSpec.hpp"
#include <algorithm>

namespace parsec::src_gen {
	void ParserSpec::defineParseRule(std::string_view name, fg::Rule rule) {
		m_parseRules.emplace_back();
		try {
			m_inputSyntax.insertSymbol(name, std::move(rule));
		} catch(...) {
			m_parseRules.pop_back();
			throw;
		}
		m_parseRules.back() = m_inputSyntax.symbols().back();
		m_parseRulesSorted = false;
	}


	bool ParserSpec::isParseRule(std::string_view name) const {
		if(std::ranges::binary_search(m_parseRules, name)) {
			return true;
		}
		return false;
	}


	std::span<const std::string_view> ParserSpec::parseRules() const {
		if(!m_parseRulesSorted) {
			std::ranges::sort(m_parseRules);
			m_parseRulesSorted = true;
		}
		return m_parseRules;
	}
}