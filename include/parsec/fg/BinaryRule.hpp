#ifndef PARSEC_FG_BINARY_RULE_HEADER
#define PARSEC_FG_BINARY_RULE_HEADER

#include "Rule.hpp"

namespace parsec::fg {
	class BinaryRule : public Rule {
	public:
		/** @{ */
		BinaryRule(RulePtr left, RulePtr right) noexcept {
			setLeft(std::move(left));
			setRight(std::move(right));
		}

		~BinaryRule() override = default;
		/** @} */


		/** @{ */
		const Rule* left() const noexcept {
			return m_left.get();
		}

		Rule* left() noexcept {
			return m_left.get();
		}


		void setLeft(RulePtr rule) noexcept {
			m_left = std::move(rule);
			m_left->setParent(this);
		}

		RulePtr takeLeft() noexcept {
			m_left->clearParent();
			return std::move(m_left);
		}
		/** @} */


		/** @{ */
		const Rule* right() const noexcept {
			return m_right.get();
		}
		
		Rule* right() noexcept {
			return m_right.get();
		}


		void setRight(RulePtr rule) noexcept {
			m_right = std::move(rule);
			m_right->setParent(this);
		}

		RulePtr takeRight() noexcept {
			m_right->clearParent();
			return std::move(m_right);
		}
		/** @} */

		
	private:
		RulePtr m_left, m_right;
	};
}

#endif