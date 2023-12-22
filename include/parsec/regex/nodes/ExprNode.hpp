#ifndef PARSEC_REGEX_NODES_EXPR_NODE_HEADER
#define PARSEC_REGEX_NODES_EXPR_NODE_HEADER

#include <ostream>
#include <memory>

namespace parsec::regex::nodes {

	class NodeVisitor;
	class BinaryExpr;
	class UnaryExpr;



	/**
	 * @brief Abstract representation of a regular expression.
	 */
	class ExprNode {
		friend BinaryExpr;
		friend UnaryExpr;
	
	public:

		friend std::ostream& operator<<(std::ostream& out, const ExprNode& n);

		friend bool operator==(const ExprNode& lhs, const ExprNode& rhs) noexcept;



		ExprNode() = default;

		ExprNode(const ExprNode&) = delete;
		ExprNode& operator=(const ExprNode&) = delete;

		virtual ~ExprNode() = default;



		/** @{ */
		/** @brief Perform an operation that depends on both the type of the expression node and the type of the NodeVisitor. */
		virtual void acceptVisitor(NodeVisitor& visitor) const = 0;


		/** @brief Calculate the total number of CharAtom%s in the expression. */
		virtual int atomCount() const noexcept = 0;
		
		
		/** @brief Parent node for the expression, if any. */
		const ExprNode* parent() const noexcept {
			return m_parent;
		}
		/** @} */



	private:

		virtual void rebaseAtomIndices(int base) noexcept = 0;

		void setParent(const ExprNode* p) noexcept {
			m_parent = p;
		}


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