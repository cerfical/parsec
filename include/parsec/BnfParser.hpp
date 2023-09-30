#ifndef PARSEC_BNF_PARSER_HEADER
#define PARSEC_BNF_PARSER_HEADER

#include "BnfLexer.hpp"
#include "LexGrammar.hpp"

#include <gsl/string_span>

#include <istream>
#include <memory>

namespace parsec {
	/**
	 * @brief Parses BNF-like descriptions of language grammars.
	 */
	class BnfParser {
	public:
		/** @{ */
		/** @brief Construct a new parser operating on a @c std::istream. */
		explicit BnfParser(std::istream& input) noexcept
		 : lexer(input)
		{ }

		/** @copybrief */
		~BnfParser() = default;
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
		LexGrammar Parse();
		/** @} */

	private:
		/** @brief Get the type of the next token without removing it from the input. */
		BnfTokenKinds PeekToken();

		/** @brief Remove the next token from the input and return it. */
		BnfToken GetToken();
		/** @brief Remove the next token from the input. */
		void SkipToken();

		/** @brief Match the next token with a text, generate an error with a message if failed. */
		BnfToken MatchToken(std::string_view text, gsl::czstring msg);
		/** @brief Match the next token with the given one, generate an error with a message if failed. */
		BnfToken MatchToken(BnfTokenKinds tok, gsl::czstring msg);

		/** @brief Parse a token definition. */
		void ParseToken();
		/** @brief Parse a list of token definitions. */
		void ParseTokenList();

		/** @brief Parse a grammar definition. */
		void ParseGrammar();

		LexGrammar grammar;
		BnfLexer lexer;
	};
}

#endif