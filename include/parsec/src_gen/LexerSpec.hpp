#ifndef PARSEC_SRC_GEN_LEXER_SPEC_HEADER
#define PARSEC_SRC_GEN_LEXER_SPEC_HEADER

#include "../core/NonCopyable.hpp"
#include "../fg/RegularGrammar.hpp"

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
		void defineToken(const std::string& name, regex::RegularExpr pattern) {
			insertToken(name, &pattern);
		}
		


		/**
		 * @brief Define a pattern-less token.
		*/
		void defineToken(const std::string& name) {
			insertToken(name, {});
		}



		/**
		 * @brief List of all tokens defined.
		*/
		std::span<const std::string_view> definedTokens() const {
			return m_tokenNames;
		}



		/**
		 * @brief Check if a token with the specified name already exists.
		*/
		bool isTokenDefined(const std::string& name) const;


		
		/**
		 * @brief Grammar description of the token language.
		*/
		const fg::RegularGrammar& tokenGrammar() const noexcept {
			return m_tokenGrammar;
		}



	private:
		void insertToken(const std::string& name, regex::RegularExpr* pattern);

		std::unordered_set<std::string> m_definedTokens;
		std::vector<std::string_view> m_tokenNames;
		fg::RegularGrammar m_tokenGrammar;
	};

}

#endif