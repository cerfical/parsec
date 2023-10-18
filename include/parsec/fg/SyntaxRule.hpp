#ifndef PARSEC_FG_SYNTAX_RULE_HEADER
#define PARSEC_FG_SYNTAX_RULE_HEADER

#include "GrammarRule.hpp"
#include "GrammarNode.hpp"

#include <memory>
#include <vector>

namespace parsec::fg {
	/**
	 * @brief Forms the syntactic structure or sentences of a language.
	 */
	class SyntaxRule : public GrammarRule {
	public:
		/** @{ */
		/** @brief Construct a new named rule with a body. */
		SyntaxRule(const std::string& name, std::unique_ptr<GrammarNode> body)
			: GrammarRule(name), m_body(std::move(body))
		{ }

	
		/** @brief Destroy the rule. */
		~SyntaxRule() override = default;
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
		void print(std::ostream& out) const override;
		/** @} */


		/** @{ */
		/** @brief Defining body of the rule. */
		const GrammarNode* body() const noexcept {
			return m_body.get();
		}
		/** @} */
		

	private:
		std::unique_ptr<GrammarNode> m_body;
	};



	/** @brief List of @ref SyntaxRule "syntax rules". */
	using SyntaxRuleList = std::vector<SyntaxRule>;
}

#endif