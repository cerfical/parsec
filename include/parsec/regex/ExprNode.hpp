#ifndef PARSEC_REGEX_EXPR_NODE_HEADER
#define PARSEC_REGEX_EXPR_NODE_HEADER

#include <ostream>
#include <memory>

namespace parsec::regex {

	class NodeVisitor;

	/**
	 * @brief Abstract representation of a regular expression.
	 */
	class ExprNode {
	public:

		friend std::ostream& operator<<(std::ostream& out, const ExprNode& n);

		friend bool operator==(const ExprNode& lhs, const ExprNode& rhs) noexcept;


		virtual ~ExprNode() = default;

		virtual void acceptVisitor(NodeVisitor& visitor) const = 0;


		/** @name Parent management */
		/** @{ */
		const ExprNode* parent() const noexcept {
			return m_parent;
		}

		void setParent(const ExprNode* parent) noexcept {
			m_parent = parent;
		}

		void clearParent() noexcept {
			setParent({});
		}
		/** @} */


	protected:
		ExprNode() = default;

		ExprNode(const ExprNode&) = default;
		ExprNode& operator=(const ExprNode&) = default;

		ExprNode(ExprNode&&) = default;
		ExprNode& operator=(ExprNode&&) = default;

	private:
		const ExprNode* m_parent = {};
	};



	/**
	 * @brief Owning pointer to an ExprNode.
	 */
	using ExprPtr = std::unique_ptr<ExprNode>;



	/**
	 * @brief Constructs an ExprNode of the specified type.
	 */
	template <typename Node, typename... Args>
		requires std::derived_from<Node, ExprNode>
	ExprPtr makeExpr(Args&&... args) {
		return std::make_unique<Node>(std::forward<Args>(args)...);
	}

}

#endif