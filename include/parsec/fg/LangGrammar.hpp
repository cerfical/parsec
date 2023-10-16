#ifndef PARSEC_FG_LANG_GRAMMAR_HEADER
#define PARSEC_FG_LANG_GRAMMAR_HEADER

#include "../regex/regex.hpp"
#include <gsl/narrow>

#include <vector>
#include <ostream>

namespace parsec::fg {
	/**
	 * @brief Named inference rule for generating grammar symbols.
	 */
	class GrammarRule {
	public:
		/** @{ */
		/** @brief Print out a rule to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const GrammarRule& r);
		/** @} */


		/** @{ */
		/** @brief Retrieve the string representation of the rule. */
		virtual std::string toStr() const = 0;
		/** @} */


		/** @{ */
		/** @brief Name of the rule. */
		const std::string& name() const noexcept {
			return m_name;
		}

		/** @brief An integer value specifying the relative priority over other rules. */
		int priority() const noexcept {
			return m_priority;
		}
		/** @} */

	protected:
		/** @{ */
		/** @brief Initialize a new rule with name and priority. */
		GrammarRule(const std::string& name, int priority)
			: m_name(name), m_priority(priority)
		{ }

		~GrammarRule() = default;
		/** @} */

		/** @{ */
		GrammarRule(const GrammarRule&) = default;
		GrammarRule& operator=(const GrammarRule&) = default;
		/** @} */

		/** @{ */
		GrammarRule(GrammarRule&&) = default;
		GrammarRule& operator=(GrammarRule&&) = default;
		/** @} */

	private:
		std::string m_name;
		int m_priority = { };
	};



	/**
	 * @brief Defines a rule for forming a single instance of a named language token.
	 */
	class TokenRule : public GrammarRule {
	public:
		/** @{ */
		/** @brief Construct a new named rule with a compiled regex pattern. */
		TokenRule(const std::string& name, regex::RegEx pattern, int priority = 0)
			: GrammarRule(name, priority), m_pattern(std::move(pattern))
		{ }
		/** @} */


		/** @{ */
		TokenRule(TokenRule&&) = default;
		TokenRule& operator=(TokenRule&&) = default;
		/** @} */

		/** @{ */
		TokenRule(const TokenRule&) = delete;
		TokenRule& operator=(const TokenRule&) = delete;
		/** @} */


		/** @{ */
		/** @brief Regex pattern defining the rule. */
		const regex::RegEx& pattern() const noexcept {
			return m_pattern;
		}
		/** @} */

		/** @{ */
		std::string toStr() const override;
		/** @} */

	private:
		regex::RegEx m_pattern;
	};

	/** @brief List of @ref TokenRule "TokenRules". */
	using TokenRuleList = std::vector<TokenRule>;



	/**
	 * @brief Defines a syntax for some language with the help of @ref GrammarRule "GrammarRules".
	 */
	class LangGrammar {
	public:
		/** @{ */
		/** @brief Construct a new empty grammar for an empty language. */
		LangGrammar() = default;
		/** @} */


		/** @{ */
		LangGrammar(LangGrammar&&) = default;
		LangGrammar& operator=(LangGrammar&&) = default;
		/** @} */

		/** @{ */
		LangGrammar(const LangGrammar&) = delete;
		LangGrammar& operator=(const LangGrammar&) = delete;
		/** @} */


		/** @{ */
		/** @brief Add a new token to the language, defined with a regex pattern. */
		void addTokenRule(const std::string& name, regex::RegEx pattern);
		/** @} */


		/** @{ */
		/** @brief List of tokens defined by the language. */
		const TokenRuleList& tokenRules() const noexcept {
			return m_tokenRules;
		}

		/** @brief Number of defined tokens in the language. */
		int tokenRuleCount() const noexcept {
			return gsl::narrow_cast<int>(m_tokenRules.size());
		}
		/** @} */

	private:
		TokenRuleList m_tokenRules;
	};
}

#endif