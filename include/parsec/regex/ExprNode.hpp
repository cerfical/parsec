#ifndef PARSEC_REGEX_EXPR_NODE_HEADER
#define PARSEC_REGEX_EXPR_NODE_HEADER

#include <ostream>
#include <memory>

namespace parsec::regex {
	class NodeVisitor;
	class ExprNode {
	public:
		/** @{ */
		friend std::ostream& operator<<(std::ostream& out, const ExprNode& n);
		/** @} */


		/** @{ */
		ExprNode() = default;

		virtual ~ExprNode() = default;
		/** @} */


		/** @{ */
		ExprNode(const ExprNode&) = delete;
		ExprNode& operator=(const ExprNode&) = delete;
		/** @} */

		/** @{ */
		ExprNode(ExprNode&&) = delete;
		ExprNode& operator=(ExprNode&&) = delete;
		/** @} */


		/** @{ */
		virtual void acceptVisitor(NodeVisitor& visitor) const = 0;
		/** @} */


		/** @{ */
		void setParent(const ExprNode* parent) noexcept {
			m_parent = parent;
		}

		void clearParent() noexcept {
			setParent(nullptr);
		}

		const ExprNode* parent() const noexcept {
			return m_parent;
		}
		/** @} */


	private:
		const ExprNode* m_parent = nullptr;
	};

	using ExprPtr = std::unique_ptr<ExprNode>;


	template <typename Node, typename... Args>
		requires std::constructible_from<Node, Args...>
	ExprPtr makeExpr(Args&&... args) {
		return std::make_unique<Node>(std::forward<Args>(args)...);
	}
}

#endif