#ifndef PARSEC_FG_PARSER_HEADER
#define PARSEC_FG_PARSER_HEADER

#include "Lexer.hpp"
#include "Grammar.hpp"
#include "SyntaxNode.hpp"

#include <unordered_map>

namespace parsec::fg {
	/**
	 * @brief Parses BNF-like definitions of language grammars.
	 */
	class Parser {
	public:
		/** @{ */
		/** @brief Set up a new ready-for-work parser. */
		Parser() = default;


		/** @brief Destroy the parser. */
		~Parser() = default;
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
		/** @} */


		/** @{ */
		bool ruleAtomStart();
		
		void parseRuleAtom();
		void parseRuleConcat();
		void parseRuleAltern();

		void parseRuleExpr();
		/** @} */


		/** @{ */
		void parseTokenDef();
		void parseTokenList();

		void parseRuleDef();
		void parseRuleList();

		void parseGrammar();
		/** @} */


		/** @{ */
		std::unique_ptr<SyntaxNode> m_ruleExpr;
		/** @} */


		/** @{ */
		std::unordered_map<std::string, std::unique_ptr<regex::ExprNode>> m_tokens;
		std::unordered_map<std::string, std::unique_ptr<SyntaxNode>> m_syntaxRules;

		Lexer m_lexer;
		/** @} */
	};
}

#endif