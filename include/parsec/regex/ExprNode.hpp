#ifndef PARSEC_REGEX_EXPR_NODE_HEADER
#define PARSEC_REGEX_EXPR_NODE_HEADER

#include <ostream>
#include <memory>

namespace parsec::regex {
	class NodeVisitor;

	/**
	 * @brief Base for all expression node classes.
	 */
	class ExprNode {
	public:
		friend std::ostream& operator<<(std::ostream& out, const ExprNode& n);


		virtual ~ExprNode() = default;

		virtual void acceptVisitor(NodeVisitor& visitor) const = 0;



		/** @{ */
		const ExprNode* parent() const noexcept {
			return m_parent;
		}

		void setParent(const ExprNode* parent) noexcept {
			m_parent = parent;
		}

		void clearParent() noexcept {
			setParent(nullptr);
		}
		/** @} */


	protected:
		ExprNode() = default;

		ExprNode(const ExprNode&) = default;
		ExprNode& operator=(const ExprNode&) = default;

		ExprNode(ExprNode&&) = default;
		ExprNode& operator=(ExprNode&&) = default;

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