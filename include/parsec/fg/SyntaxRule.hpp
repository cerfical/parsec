#ifndef PARSEC_FG_SYNTAX_RULE_HEADER
#define PARSEC_FG_SYNTAX_RULE_HEADER

#include "SyntaxNode.hpp"

#include <memory>
#include <vector>

namespace parsec::fg {
	/**
	 * @brief Inference rules for describing the syntactic structure of a language.
	 */
	class SyntaxRule {
	public:
		/** @{ */
		/** @brief Output stream operator for syntax rules. */
		friend std::ostream& operator<<(std::ostream& out, const SyntaxRule& r);
		/** @} */


		/** @{ */
		/** @brief Construct a new named rule with a body. */
		SyntaxRule(const std::string& name, std::unique_ptr<SyntaxNode> body)
			: m_name(name), m_body(std::move(body))
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
		/** @brief Name of the rule. */
		const std::string& name() const noexcept {
			return m_name;
		}


		/** @brief Defining body of the rule. */
		const SyntaxNode* body() const noexcept {
			return m_body.get();
		}
		/** @} */
		

	private:
		/** @{ */
		std::unique_ptr<SyntaxNode> m_body;
		std::string m_name;
		/** @} */
	};



	/** @brief List of syntax rules. */
	using SyntaxRuleList = std::vector<SyntaxRule>;
}

#endif