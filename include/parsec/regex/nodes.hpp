#ifndef PARSEC_REGEX_NODES_HEADER
#define PARSEC_REGEX_NODES_HEADER

#include "ExprNode.hpp"
#include <memory>

namespace parsec::regex {
	/**
	 * @brief Literal character regular expression.
	 */
	class CharLiteral : public ExprNode {
	public:
		/** @{ */
		/** @brief Construct a new node with a value. */
		explicit CharLiteral(char value) noexcept
			: m_value(value)
		{ }
		/** @} */


		/** @{ */
		void traverseWith(Traverser& traverser) const override;
		bool nullable() const noexcept override;
		/** @} */


		/** @{ */
		/** @brief Value of the character literal. */
		char value() const noexcept {
			return m_value;
		}

		/** @brief Find the nodes that can take the position after this node. */
		CharLiteralList computeFollowPos() const;
		/** @} */

	private:
		char m_value;
	};



	/**
	 * @brief Empty regular expression.
	 */
	class NilExpr : public ExprNode {
	public:
		/** @{ */
		/** @brief Construct a new node. */
		NilExpr() = default;
		/** @} */

		/** @{ */
		void traverseWith(Traverser& traverser) const override;
		bool nullable() const noexcept override;
		/** @} */
	};



	/**
	 * @brief Unary regex operator.
	 */
	class UnaryExpr : public ExprNode {
	public:
		/** @{ */
		/** @brief Construct a new node with a single subexpression. */
		explicit UnaryExpr(std::unique_ptr<ExprNode> inner) noexcept
			: m_inner(std::move(inner)) {
			m_inner->setParent(this);
		}
		/** @} */

		/** @{ */
		/** @brief Inner subexpression of the expression represented by the node. */
		const ExprNode* innerSubexpr() const noexcept {
			return m_inner.get();
		}
		/** @} */
		
	private:
		std::unique_ptr<ExprNode> m_inner;
	};



	/**
	 * @brief Optional regular expression.
	 */
	class OptionalExpr : public UnaryExpr {
	public:
		/** @{ */
		/** @brief Construct a new node with a single subexpression. */
		explicit OptionalExpr(std::unique_ptr<ExprNode> inner) noexcept
			: UnaryExpr(std::move(inner))
		{ }
		/** @} */

		/** @{ */
		void traverseWith(Traverser& traverser) const override;
		bool nullable() const noexcept override;
		/** @} */
	};



	/**
	 * @brief Kleene plus expression.
	 */
	class PlusExpr : public UnaryExpr {
	public:
		/** @{ */
		/** @brief Construct a new node with a single subexpression. */
		explicit PlusExpr(std::unique_ptr<ExprNode> inner) noexcept
			: UnaryExpr(std::move(inner))
		{ }
		/** @} */

		/** @{ */
		void traverseWith(Traverser& traverser) const override;
		bool nullable() const noexcept override;
		/** @} */
	};



	/**
	 * @brief Kleene star expression.
	 */
	class StarExpr : public UnaryExpr {
	public:
		/** @{ */
		/** @brief Construct a new node with a single subexpression. */
		explicit StarExpr(std::unique_ptr<ExprNode> inner) noexcept
			: UnaryExpr(std::move(inner))
		{ }
		/** @} */

		/** @{ */
		void traverseWith(Traverser& traverser) const override;
		bool nullable() const noexcept override;
		/** @} */
	};



	/**
	 * @brief Binary regex operator.
	 */
	class BinaryExpr : public ExprNode {
	public:
		/** @{ */
		/** @brief Construct a new node node with two subexpressions. */
		BinaryExpr(std::unique_ptr<ExprNode> left, std::unique_ptr<ExprNode> right) noexcept
			: m_left(std::move(left)), m_right(std::move(right)) {
			m_left->setParent(this);
			m_right->setParent(this);
		}
		/** @} */

		/** @{ */
		/** @brief Left subexpression of the expression represented by the node. */
		const ExprNode* leftSubexpr() const noexcept {
			return m_left.get();
		}

		/** @brief Right subexpression of the expression represented by the node. */
		const ExprNode* rightSubexpr() const noexcept {
			return m_right.get();
		}
		/** @} */

	private:
		std::unique_ptr<ExprNode> m_left, m_right;
	};



	/**
	 * @brief Alternation expression.
	 */
	class AlternExpr : public BinaryExpr {
	public:
		/** @{ */
		/** @brief Construct a new node node with two subexpressions. */
		AlternExpr(std::unique_ptr<ExprNode> left, std::unique_ptr<ExprNode> right) noexcept
			: BinaryExpr(std::move(left), std::move(right))
		{ }
		/** @} */

		/** @{ */
		void traverseWith(Traverser& traverser) const override;
		bool nullable() const noexcept override;
		/** @} */
	};



	/**
	 * @brief Concatenation expression.
	 */
	class ConcatExpr : public BinaryExpr {
	public:
		/** @{ */
		/** @brief Construct a new node node with two subexpressions. */
		ConcatExpr(std::unique_ptr<ExprNode> left, std::unique_ptr<ExprNode> right) noexcept
			: BinaryExpr(std::move(left), std::move(right))
		{ }
		/** @} */

		/** @{ */
		void traverseWith(Traverser& traverser) const override;
		bool nullable() const noexcept override;
		/** @} */
	};
}

#endif