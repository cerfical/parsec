#ifndef PARSEC_FG_UNARY_RULE_HEADER
#define PARSEC_FG_UNARY_RULE_HEADER

#include "Rule.hpp"

namespace parsec::fg {
	class UnaryRule : public Rule {
	public:
		/** @{ */
		explicit UnaryRule(RulePtr inner) noexcept
			: m_inner(std::move(inner)) {
			m_inner->setParent(this);
		}

		~UnaryRule() override = default;
		/** @} */


		/** @{ */
		const Rule* inner() const noexcept {
			return m_inner.get();
		}
		/** @} */


	private:
		/** @{ */
		RulePtr m_inner;
		/** @} */
	};
}

#endif