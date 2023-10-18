#ifndef PARSEC_FG_TOKEN_RULE_HEADER
#define PARSEC_FG_TOKEN_RULE_HEADER

#include "../regex/regex.hpp"

#include "GrammarRule.hpp"
#include <vector>

namespace parsec::fg {
	/**
	 * @brief Defines a rule for forming a single instance of a named language token.
	 */
	class TokenRule : public GrammarRule {
	public:
		/** @{ */
		/** @brief Construct a new named rule with a compiled regex pattern. */
		TokenRule(const std::string& name, regex::RegEx pattern, int priority = 0)
			: GrammarRule(name), m_pattern(std::move(pattern)), m_priority(priority)
		{ }


		/** @brief Destroy the rule. */
		~TokenRule() override = default;
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
		void print(std::ostream& out) const override;
		/** @} */


		/** @{ */
		/** @brief Regex pattern defining the rule. */
		const regex::RegEx& pattern() const noexcept {
			return m_pattern;
		}


		/** @brief Defines the use priority relative to other rules. */
		int priority() const noexcept {
			return m_priority;
		}
		/** @} */


	private:
		regex::RegEx m_pattern;
		int m_priority = 0;
	};



	/** @brief List of @ref TokenRule "token rules". */
	using TokenRuleList = std::vector<TokenRule>;
}

#endif