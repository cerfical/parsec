#ifndef PARSEC_SRC_GEN_LEXER_SPEC_HEADER
#define PARSEC_SRC_GEN_LEXER_SPEC_HEADER

#include "../core/NonCopyable.hpp"
#include "../regex/RegularExpr.hpp"
#include "../fg/SymbolGrammar.hpp"

#include <unordered_set>
#include <span>
#include <vector>

namespace parsec::src_gen {

	/**
	 * @brief Specification of a lexical analyzer to generate.
	*/
	class LexerSpec : private NonCopyable {
	public:
		
		/**
		 * @brief Specify a new named token to be recognized by the lexer.
		*/
		void defineToken(std::string_view name, const regex::RegularExpr& pattern);
		


		/**
		 * @brief Check whether the specified name defines a token.
		*/
		bool isToken(std::string_view name) const {
			if(m_tokens.resolveSymbol(name)) {
				return true;
			}
			return false;
		}



		/**
		 * @brief Grammar description of the token language.
		*/
		const fg::SymbolGrammar& tokens() const {
			return m_tokens;
		}


	private:
		fg::SymbolGrammar m_tokens;
	};

}

#endif