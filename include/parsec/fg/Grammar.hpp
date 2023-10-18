#ifndef PARSEC_FG_GRAMMAR_HEADER
#define PARSEC_FG_GRAMMAR_HEADER

#include "TokenRule.hpp"
#include "SyntaxRule.hpp"

namespace parsec::fg {
	/**
	 * @brief Defines a syntax for some language with the help of @ref GrammarRule "grammar rules".
	 */
	class Grammar {
	public:
		/** @{ */
		/** @brief Construct a new empty grammar for an empty language. */
		Grammar() = default;


		/** @brief Destroy the grammar. */
		~Grammar() = default;
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


		/** @brief List of @ref TokenRule "tokens" defined by the language. */
		const TokenRuleList& tokenRules() const noexcept {
			return m_tokenRules;
		}
		/** @} */


		/** @{ */
		/** @brief Add a new @ref SyntaxRule "syntax rule" to the grammar. */
		void addSyntaxRule(const std::string& name, std::unique_ptr<SyntaxNode> body);


		/** @brief List of @ref SyntaxRule "syntax rules" forming the language. */
		const SyntaxRuleList& syntaxRules() const noexcept {
			return m_syntaxRules;
		}
		/** @} */


	private:
		SyntaxRuleList m_syntaxRules;
		TokenRuleList m_tokenRules;
	};
}

#endif