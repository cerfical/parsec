#ifndef PARSEC_FG_RULES_BINARY_RULE_HEADER
#define PARSEC_FG_RULES_BINARY_RULE_HEADER

#include "RuleNode.hpp"
#include <memory>

namespace parsec::fg::rules {
	class BinaryRule : public RuleNode {
	public:
		/** @{ */
		BinaryRule(std::unique_ptr<RuleNode> left, std::unique_ptr<RuleNode> right) noexcept
			: m_left(std::move(left)), m_right(std::move(right)) {
			m_left->setParent(this);
			m_right->setParent(this);
		}

		~BinaryRule() override = default;
		/** @} */


		/** @{ */
		const RuleNode* left() const noexcept {
			return m_left.get();
		}

		const RuleNode* right() const noexcept {
			return m_right.get();
		}
		/** @} */


	private:
		/** @{ */
		std::unique_ptr<RuleNode> m_left;
		std::unique_ptr<RuleNode> m_right;
		/** @} */
	};
}

#endif