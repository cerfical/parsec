#ifndef PARSEC_FG_GRAMMAR_HEADER
#define PARSEC_FG_GRAMMAR_HEADER

#include "TokenRule.hpp"
#include "SyntaxRule.hpp"

#include <unordered_map>

namespace parsec::fg {
	/**
	 * @brief Defines a syntax for some language with the help of @ref GrammarRule "grammar rules".
	 */
	class Grammar {
	public:
		/** @{ */
		Grammar() = default;
		/** @} */


		/** @{ */
		Grammar(Grammar&&) = default;
		Grammar& operator=(Grammar&&) = default;
		/** @} */


		/** @{ */
		Grammar(const Grammar&) = delete;
		Grammar& operator=(const Grammar&) = delete;
		/** @} */


		/** @{ */
		/** @brief Add a new @ref TokenRule "token" to the language. */
		void addTokenRule(const std::string& name, std::unique_ptr<regex::ExprNode> pattern);


		/** @brief Add a new @ref SyntaxRule "syntax rule" to the grammar. */
		void addSyntaxRule(const std::string& name, std::unique_ptr<SyntaxNode> body);
		/** @} */


		/** @{ */
		/** @brief Finds the rule with the specified name. */
		const GrammarRule* resolveRule(const std::string& ruleName) const;


		/** @brief List of all @ref TokenRule "tokens" defined by the grammar. */
		const TokenRuleList& tokens() const noexcept {
			return m_tokens;
		}


		/** @brief List of all @ref SyntaxRule "syntax rules" defined by the grammar. */
		const SyntaxRuleList& syntax() const noexcept {
			return m_syntax;
		}
		/** @} */


	private:
		/** @{ */
		std::unordered_map<std::string, std::pair<std::size_t, bool>> m_rules;

		SyntaxRuleList m_syntax;
		TokenRuleList m_tokens;
		/** @} */
	};
}

#endif