#ifndef PARSEC_FG_TOKEN_RULE_HEADER
#define PARSEC_FG_TOKEN_RULE_HEADER

#include "GrammarRule.hpp"
#include <memory>
#include <vector>

namespace parsec::fg {
	/**
	 * @brief Defines a rule for forming a single instance of a named language token.
	 */
	class TokenRule : public GrammarRule {
	public:
		/** @{ */
		/** @brief Output stream operator for token rules. */
		friend std::ostream& operator<<(std::ostream& out, const TokenRule& r);
		/** @} */


		/** @{ */
		TokenRule() = default;

		/** @brief Construct a new rule from a regex pattern. */
		TokenRule(const std::string& name, std::unique_ptr<int> pattern, int priority);
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
		int pattern() const noexcept {
			return 0;
		}


		/** @brief Special position in the regex pattern that identifies its end. */
		int endMarker() const noexcept {
			return 0;
		}


		/** @brief Defines the use priority relative to other rules. */
		int priority() const noexcept {
			return m_priority;
		}
		/** @} */


	private:
		/** @{ */
		std::unique_ptr<int> m_pattern;
		int m_priority = 0;
		/** @} */
	};



	/** @brief List of @ref TokenRule "token rules". */
	using TokenRuleList = std::vector<TokenRule>;
}

#endif