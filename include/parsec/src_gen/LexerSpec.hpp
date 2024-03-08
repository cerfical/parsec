#ifndef PARSEC_SRC_GEN_LEXER_SPEC_HEADER
#define PARSEC_SRC_GEN_LEXER_SPEC_HEADER

#include "../core/NonCopyable.hpp"
#include "../regex/RegularExpr.hpp"
#include "../fg/SymbolGrammar.hpp"

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
		bool isToken(std::string_view name) const;



		/**
		 * @brief Lexicographically sorted list of all defined tokens.
		*/
		std::span<const std::string_view> tokens() const;



		/**
		 * @brief Grammar description of the lexer input language.
		*/
		const fg::SymbolGrammar& inputSyntax() const {
			return m_inputSyntax;
		}



	private:
		fg::SymbolGrammar m_inputSyntax;

		mutable std::vector<std::string_view> m_tokens;
		mutable bool m_tokensSorted = true;
	};

}

#endif