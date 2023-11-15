#ifndef PARSEC_FG_UNARY_RULE_HEADER
#define PARSEC_FG_UNARY_RULE_HEADER

#include "Rule.hpp"

namespace parsec::fg {
	class UnaryRule : public Rule {
	public:
		/** @{ */
		explicit UnaryRule(RulePtr inner) noexcept {
			setInner(std::move(inner));
		}

		~UnaryRule() override = default;
		/** @} */


		/** @{ */
		const Rule* inner() const noexcept {
			return m_inner.get();
		}

		Rule* inner() noexcept {
			return m_inner.get();
		}


		void setInner(RulePtr rule) noexcept {
			m_inner = std::move(rule);
			m_inner->setParent(this);
		}

		RulePtr takeInner() noexcept {
			m_inner->clearParent();
			return std::move(m_inner);
		}
		/** @} */


	private:
		RulePtr m_inner;
	};
}

#endif