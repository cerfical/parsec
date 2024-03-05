#include "src_gen/LexerSpec.hpp"
#include "utils/string_utils.hpp"

namespace parsec::src_gen {
	std::string LexerSpec::canonicalizeName(std::string_view name) {
		return string_utils::toPascalCase(name);
	}
	
	void LexerSpec::insertToken(std::string_view name, regex::RegularExpr* pattern) {
		const auto [it, wasInserted] = m_definedTokens.insert(canonicalizeName(name));
		const auto& tokenName = *it;

		if(wasInserted) {
			try {
				m_tokenNames.emplace_back(tokenName);
			} catch(...) {
				m_definedTokens.erase(it);
				throw;
			}
		}

		if(pattern) {
			try {
				m_tokenGrammar.addPattern(tokenName, std::move(*pattern));
			} catch(...) {
				m_tokenNames.pop_back();
				if(wasInserted) {
					m_definedTokens.erase(it);
				}
				throw;
			}
		}
	}
}