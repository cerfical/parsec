#ifndef PARSEC_FG_BINARY_RULE_HEADER
#define PARSEC_FG_BINARY_RULE_HEADER

#include "Rule.hpp"

namespace parsec::fg {
	class BinaryRule : public Rule {
	public:
		/** @{ */
		BinaryRule(RulePtr left, RulePtr right) noexcept
			: m_left(std::move(left)), m_right(std::move(right)) {
			m_left->setParent(this);
			m_right->setParent(this);
		}

		~BinaryRule() override = default;
		/** @} */


		/** @{ */
		const Rule* left() const noexcept {
			return m_left.get();
		}

		RulePtr& left() noexcept {
			return m_left;
		}
		/** @} */


		/** @{ */
		const Rule* right() const noexcept {
			return m_right.get();
		}
		
		RulePtr& right() noexcept {
			return m_right;
		}
		/** @} */

		
	private:
		RulePtr m_left;
		RulePtr m_right;
	};
}

#endif