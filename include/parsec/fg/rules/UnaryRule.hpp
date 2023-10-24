#ifndef PARSEC_FG_RULES_UNARY_RULE_HEADER
#define PARSEC_FG_RULES_UNARY_RULE_HEADER

#include "RuleNode.hpp"
#include <memory>

namespace parsec::fg::rules {
	class UnaryRule : public RuleNode {
	public:
		/** @{ */
		explicit UnaryRule(std::unique_ptr<RuleNode> inner) noexcept
			: m_inner(std::move(inner)) {
			m_inner->setParent(this);
		}

		~UnaryRule() override = default;
		/** @} */


		/** @{ */
		const RuleNode* inner() const noexcept {
			return m_inner.get();
		}
		/** @} */


	private:
		/** @{ */
		std::unique_ptr<RuleNode> m_inner;
		/** @} */
	};
}

#endif