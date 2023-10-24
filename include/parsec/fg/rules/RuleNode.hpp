#ifndef PARSEC_FG_RULES_RULE_NODE_HEADER
#define PARSEC_FG_RULES_RULE_NODE_HEADER

#include <ostream>
#include <vector>

namespace parsec::fg::rules {
	class RuleTraverser;
	class RulePos;

	using RulePosList = std::vector<const RulePos*>;


	class RuleNode {
	public:
		/** @{ */
		friend std::ostream& operator<<(std::ostream& out, const RuleNode& n);
		/** @} */


		/** @{ */
		RuleNode() = default;
		virtual ~RuleNode() = default;
		/** @} */


		/** @{ */
		RuleNode(const RuleNode&) = delete;
		RuleNode& operator=(const RuleNode&) = delete;
		/** @} */


		/** @{ */
		RuleNode(RuleNode&&) = delete;
		RuleNode& operator=(RuleNode&&) = delete;
		/** @} */


		/** @{ */
		virtual void traverseWith(RuleTraverser& traverser) const = 0;
		virtual bool nullable() const noexcept = 0;
		/** @} */


		/** @{ */
		void setParent(const RuleNode* parent) noexcept {
			m_parent = parent;
		}

		const RuleNode* parent() const noexcept {
			return m_parent;
		}
		/** @} */


		/** @{ */
		RulePosList firstPos() const;
		RulePosList lastPos() const;
		/** @} */


	private:
		/** @{ */
		const RuleNode* m_parent = nullptr;
		/** @} */
	};
}

#endif