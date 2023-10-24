#ifndef PARSEC_FG_RULES_RULE_REF_HEADER
#define PARSEC_FG_RULES_RULE_REF_HEADER

#include "RulePos.hpp"
#include <string>

namespace parsec::fg::rules {
	class RuleRef : public RulePos {
	public:
		/** @{ */
		explicit RuleRef(const std::string& ruleName)
			: m_ruleName(ruleName)
		{ }

		~RuleRef() override = default;
		/** @} */


		/** @{ */
		void traverseWith(RuleTraverser& traverser) const override;
		/** @} */


		/** @{ */
		const std::string& ruleName() const noexcept {
			return m_ruleName;
		}
		/** @} */


	private:
		/** @{ */
		std::string m_ruleName;
		/** @} */
	};
}

#endif