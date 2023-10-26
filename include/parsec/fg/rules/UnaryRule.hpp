#ifndef PARSEC_FG_RULES_UNARY_RULE_HEADER
#define PARSEC_FG_RULES_UNARY_RULE_HEADER

#include "Rule.hpp"
#include <memory>

namespace parsec::fg::rules {
	class UnaryRule : public Rule {
	public:
		/** @{ */
		explicit UnaryRule(std::unique_ptr<Rule> inner) noexcept
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
		std::unique_ptr<Rule> m_inner;
		/** @} */
	};
}

#endif