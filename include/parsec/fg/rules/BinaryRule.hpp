#ifndef PARSEC_FG_RULES_BINARY_RULE_HEADER
#define PARSEC_FG_RULES_BINARY_RULE_HEADER

#include "Rule.hpp"
#include <memory>

namespace parsec::fg::rules {
	class BinaryRule : public Rule {
	public:
		/** @{ */
		BinaryRule(std::unique_ptr<Rule> left, std::unique_ptr<Rule> right) noexcept
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

		const Rule* right() const noexcept {
			return m_right.get();
		}
		/** @} */


	private:
		/** @{ */
		std::unique_ptr<Rule> m_left;
		std::unique_ptr<Rule> m_right;
		/** @} */
	};
}

#endif