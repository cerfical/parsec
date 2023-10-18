#ifndef PARSEC_REGEX_EXPR_NODE_HEADER
#define PARSEC_REGEX_EXPR_NODE_HEADER

#include <ostream>
#include <vector>

namespace parsec::regex {
	class NodeTraverser;
	class CharLiteral;

	/** @brief List of @ref CharLiteral "character literals". */
	using CharLiteralList = std::vector<const CharLiteral*>;



	/**
	 * @brief Polymorphic entry point to the tree representation of a regular expression.
	 */
	class ExprNode {
	public:
		/** @{ */
		/** @brief Output stream operator for a node. */
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
		/** @brief Traverse the regex tree in a type-safe manner. */
		virtual void traverseWith(NodeTraverser& traverser) const = 0;


		/** @brief Indicates whether the node can generate an empty string. */
		virtual bool nullable() const noexcept = 0;
		/** @} */


		/** @{ */
		/** @brief Set the new parent node. */
		void setParent(const ExprNode* parent) noexcept {
			m_parent = parent;
		}


		/** @brief Parent node. */
		const ExprNode* parent() const noexcept {
			return m_parent;
		}
		/** @} */


		/** @{ */
		/** @brief Finds the nodes taking the first position in the generated string. */
		CharLiteralList firstPos() const;


		/** @brief Finds the nodes taking the last position in the generated string. */
		CharLiteralList lastPos() const;
		/** @} */


	private:
		const ExprNode* m_parent = nullptr;
	};
}

#endif