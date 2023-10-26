#ifndef PARSEC_FG_SYNTAX_RULE_HEADER
#define PARSEC_FG_SYNTAX_RULE_HEADER

#include "rules/Rule.hpp"
#include "GrammarRule.hpp"

#include <memory>
#include <vector>

namespace parsec::fg {
	/**
	 * @brief Inference rules for describing the syntactic structure of a language.
	 */
	class SyntaxRule : public GrammarRule {
	public:
		/** @{ */
		/** @brief Output stream operator for syntax rules. */
		friend std::ostream& operator<<(std::ostream& out, const SyntaxRule& r);
		/** @} */


		/** @{ */
		SyntaxRule() = default;

		/** @brief Construct a new named rule with a body. */
		SyntaxRule(const std::string& name, std::unique_ptr<rules::Rule> body)
			: GrammarRule(name), m_body(std::move(body))
		{ }
		/** @} */


		/** @{ */
		SyntaxRule(SyntaxRule&&) = default;
		SyntaxRule& operator=(SyntaxRule&&) = default;
		/** @} */


		/** @{ */
		SyntaxRule(const SyntaxRule&) = delete;
		SyntaxRule& operator=(const SyntaxRule&) = delete;
		/** @} */


		/** @{ */
		/** @brief Defining body of the rule. */
		const rules::Rule* body() const noexcept {
			return m_body.get();
		}
		/** @} */
		

	private:
		/** @{ */
		std::unique_ptr<rules::Rule> m_body;
		/** @} */
	};



	/** @brief List of @ref SyntaxRule "syntax rules". */
	using SyntaxRuleList = std::vector<SyntaxRule>;
}

#endif