#ifndef PARSEC_SRC_GEN_LEXER_SPEC_HEADER
#define PARSEC_SRC_GEN_LEXER_SPEC_HEADER

#include "../core/NonCopyable.hpp"
#include "../fg/RegularGrammar.hpp"

#include <unordered_set>
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
		void defineToken(std::string_view name, regex::RegularExpr pattern) {
			insertToken(name, &pattern);
		}
		


		/**
		 * @brief Define a pattern-less token.
		*/
		void defineToken(std::string_view name) {
			insertToken(name, {});
		}



		/**
		 * @brief Define an unnamed token.
		*/
		void defineToken(regex::RegularExpr pattern) {
			insertToken("", &pattern);
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
		bool isTokenDefined(std::string_view name) const {
			return m_definedTokens.contains(canonicalizeName(name));
		}


		
		/**
		 * @brief Grammar description of the token language.
		*/
		const fg::RegularGrammar& tokenGrammar() const noexcept {
			return m_tokenGrammar;
		}



	private:
		
		static std::string canonicalizeName(std::string_view name);

		void insertToken(std::string_view name, regex::RegularExpr* pattern);


		std::unordered_set<std::string> m_definedTokens;
		std::vector<std::string_view> m_tokenNames;
		fg::RegularGrammar m_tokenGrammar;
	};

}

#endif