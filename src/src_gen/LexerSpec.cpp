#include "src_gen/LexerSpec.hpp"
#include "src_gen/utils.hpp"

namespace parsec::src_gen {
	void LexerSpec::insertToken(const std::string& name, regex::RegularExpr* pattern) {
		const auto [it, wasInserted] = m_definedTokens.insert(utils::normalizeName(name));
		const auto& normalizedName = *it;

		if(wasInserted) {
			try {
				m_tokenNames.emplace_back(normalizedName);
			} catch(...) {
				m_definedTokens.erase(it);
				throw;
			}
		}

		if(pattern) {
			try {
				m_tokenGrammar.addPattern(name, std::move(*pattern));
			} catch(...) {
				if(wasInserted) {
					m_tokenNames.pop_back();
					m_definedTokens.erase(it);
				}
				throw;
			}
		}
	}

	bool LexerSpec::isTokenDefined(const std::string& name) const {
		return m_definedTokens.contains(utils::normalizeName(name));
	}
}