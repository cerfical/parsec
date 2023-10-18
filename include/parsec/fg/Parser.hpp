#ifndef PARSEC_FG_PARSER_HEADER
#define PARSEC_FG_PARSER_HEADER

#include "Lexer.hpp"
#include "Grammar.hpp"

#include <gsl/gsl>

namespace parsec::fg {
	/**
	 * @brief Parses BNF-like definitions of language grammars.
	 */
	class Parser {
	public:
		/** @{ */
		Parser() = default;
		/** @} */


		/** @{ */
		Parser(Parser&&) = default;
		Parser& operator=(Parser&&) = default;
		/** @} */


		/** @{ */
		Parser(const Parser&) = delete;
		Parser& operator=(const Parser&) = delete;
		/** @} */


		/** @{ */
		/** @brief Parses an arbitrary sequence of characters for a valid grammar. */
		Grammar parse(std::string_view input);


		/** @brief Parses characters from a @c std::istream for a valid grammar. */
		Grammar parse(std::istream& input);
		/** @} */


	private:
		/** @{ */
		[[noreturn]] void parseError(gsl::czstring msg);
		[[noreturn]] void badTokenError();
		/** @} */


		/** @{ */
		std::unique_ptr<regex::ExprNode> parseRegex(const Token& regex);

		void parseTokenDef();
		void parseTokenList();
		/** @} */


		/** @{ */
		bool atomStart();
		
		void parseAtom();
		void parseConcat();
		void parseAltern();

		void parseRule();

		void parseRuleDef();
		void parseRuleList();
		/** @} */


		/** @{ */
		void parseGrammar();
		/** @} */


		/** @{ */
		std::unique_ptr<SyntaxNode> m_rule;
		Grammar m_grammar;
		/** @} */


		/** @{ */
		Lexer m_lexer;
		/** @} */
	};
}

#endif