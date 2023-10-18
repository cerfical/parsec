#ifndef PARSEC_FG_RULE_REF_HEADER
#define PARSEC_FG_RULE_REF_HEADER

#include "SyntaxNode.hpp"
#include <string>

namespace parsec::fg {
	/**
	 * @brief Reference to a grammar rule defined elsewhere.
	 */
	class RuleRef : public SyntaxNode {
	public:
		/** @{ */
		/** @brief Define a new reference to a grammar rule. */
		explicit RuleRef(const std::string& ruleName)
			: m_ruleName(ruleName)
		{ }

		~RuleRef() override = default;
		/** @} */


		/** @{ */
		/** @brief Name of the referenced rule. */
		const std::string& ruleName() const noexcept {
			return m_ruleName;
		}
		/** @} */


		/** @{ */
		void traverseWith(NodeTraverser& traverser) const override;
		/** @} */


	private:
		std::string m_ruleName;
	};
}

#endif