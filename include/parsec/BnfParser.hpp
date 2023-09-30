#ifndef PARSEC_BNF_PARSER_HEADER
#define PARSEC_BNF_PARSER_HEADER

#include "BnfLexer.hpp"
#include "Grammar.hpp"

#include <unordered_map>

namespace parsec {
	/**
	 * @brief Parses BNF-like descriptions of language grammars.
	 */
	class BnfParser {
	public:
		/** @{ */
		/** @brief Construct a defaulted parser that does nothing. */
		BnfParser() = default;

		/** @brief Construct a new parser operating on a @c std::istream. */
		explicit BnfParser(std::istream* input) noexcept
		 : m_lexer(input)
		{ }
		/** @} */

		/** @{ */
		BnfParser(BnfParser&&) = default;
		BnfParser& operator=(BnfParser&&) = default;
		/** @} */

		/** @{ */
		BnfParser(const BnfParser&) = delete;
		BnfParser& operator=(const BnfParser&) = delete;
		/** @} */

		/** @{ */
		/** @brief Parse the input for a valid grammar description. */
		Grammar Parse();
		/** @} */

	private:
		/** @{ */
		[[noreturn]] void UnexpectedToken();
		/** @} */

		/** @{ */
		BnfTokenKinds PeekToken();
		BnfToken GetToken();

		bool SkipKeywordIf(std::string_view keyword);
		void SkipToken();
		/** @} */

		/** @{ */
		BnfToken MatchToken(BnfTokenKinds tok, gsl::czstring msg);
		std::unique_ptr<RegExNode> ParseRegex(const BnfToken& regex);
		/** @} */

		/** @{ */
		void ParseToken();
		void ParseTokenList();
		void ParseGrammar();
		/** @} */

		/** @{ */
		void PopulateGrammar(Grammar& grammar);
		/** @} */

		std::unordered_map<std::string, std::unique_ptr<RegExNode>> m_tokens;
		BnfLexer m_lexer;
	};
}

#endif