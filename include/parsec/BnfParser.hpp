#ifndef PARSEC_BNF_PARSER_HEADER
#define PARSEC_BNF_PARSER_HEADER

#include "BnfLexer.hpp"
#include "Utils.hpp"

#include <gsl/gsl>
#include <istream>

namespace parsec {
	/**
	 * @brief Parses BNF-like descriptions of some language grammars.
	 */
	class BnfParser {
	public:
		/** @{ */
		/** @brief Construct a new BnfParser that operates on @c std::istream. */
		explicit BnfParser(std::istream& input) noexcept
		 : lexer(input)
		{ }

		/** @brief Destroy the BnfParser. */
		~BnfParser() = default;
		/** @} */

		/** @{ */
		/** @brief Construct a new BnfParser by moving from another BnfParser. */
		BnfParser(BnfParser&&) = default;

		/** @brief Move another BnfParser into the BnfParser. */
		BnfParser& operator=(BnfParser&&) = default;
		/** @} */

		/** @{ */
		BnfParser(const BnfParser&) = delete;
		BnfParser& operator=(const BnfParser&) = delete;
		/** @} */

		/** @{ */
		/** @brief Parse the input given to the BnfParser. */
		void Parse();
		/** @} */

	private:
		/** @brief Get the type of the next token without removing it from the input. */
		BnfTokenKinds PeekToken();

		/** @brief Remove the next token from the input and return it. */
		BnfToken GetToken();
		/** @brief Remove the next token from the input. */
		void SkipToken();

		/** @brief Tries to match the next token with the given text, otherwise generates an error with specifed message. */
		void MatchToken(std::string_view text, gsl::czstring msg);
		/** @brief Tries to match the next token with the given one, otherwise generates an error with specifed message. */
		void MatchToken(BnfTokenKinds tok, gsl::czstring msg);

		/** @brief Parse a token definition. */
		void ParseTokenDef();
		/** @brief Parse a list of token definitions. */
		void ParseTokenList();

		/** @brief Parse a grammar definition. */
		void ParseGrammarDef();

		BnfLexer lexer;
	};
}

#endif