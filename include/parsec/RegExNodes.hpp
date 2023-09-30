#ifndef PARSEC_REGEX_NODES_HEADER
#define PARSEC_REGEX_NODES_HEADER

#include <iostream>
#include <memory>

namespace parsec {
	class RegExTraverser;
	/**
	 * @brief Polymorphic entry point to the tree representation of a regular expression.
	 */
	class RegExNode {
	public:
		/** @{ */
		RegExNode(const RegExNode&) = delete;
		RegExNode& operator=(const RegExNode&) = delete;
		/** @} */

		/** @{ */
		RegExNode(RegExNode&&) = delete;
		RegExNode& operator=(RegExNode&&) = delete;
		/** @} */

		/** @{ */
		/** @copybrief */
		virtual ~RegExNode() = default;
		/** @} */

		/** @{ */
		/** @brief Examine the node in a type-safe manner using the visitor pattern. */
		virtual void Traverse(RegExTraverser& traverser) const = 0;
		
		/** @brief Print out the node to a @c std::ostream. */
		void Print(std::ostream& out = std::cout) const;
		/** @} */

	protected:
		/** @copybrief */
		RegExNode() = default;
	};



	/**
	 * @brief Literal regular expression.
	 */
	class RegExLiteral : public RegExNode {
	public:
		/** @{ */
		/** @brief Construct a new RegExLiteral from its value. */
		explicit RegExLiteral(char value) noexcept
		 : value(value)
		{ }

		/** @copybrief */
		~RegExLiteral() = default;
		/** @} */

		/** @{ */
		void Traverse(RegExTraverser& traverser) const override;
		/** @} */

		/** @{ */
		/** @brief Get the value of the RegExLiteral. */
		char GetValue() const noexcept {
			return value;
		}
		/** @} */

	private:
		char value;
	};



	/**
	 * @brief Kleene star expression.
	 */
	class RegExStar : public RegExNode {
	public:
		/** @{ */
		/** @brief Construct a new RegExStar from its inner expression. */
		explicit RegExStar(std::unique_ptr<RegExNode> expr) noexcept
		 : innerExpr(std::move(expr))
		{ }

		/** @copybrief */
		~RegExStar() = default;
		/** @} */

		/** @{ */
		void Traverse(RegExTraverser& traverser) const override;
		/** @} */

		/** @{ */
		/** @brief Get the inner expression of the RegExStar. */
		const RegExNode& GetInnerExpr() const noexcept {
			return *innerExpr;
		}
		/** @} */

	private:
		std::unique_ptr<RegExNode> innerExpr;
	};



	/**
	 * @brief Alternation expression.
	 */
	class RegExAltern : public RegExNode {
	public:
		/** @{ */
		/** @brief Construct a new RegExAltern from its subexpressions. */
		RegExAltern(std::unique_ptr<RegExNode> left, std::unique_ptr<RegExNode> right) noexcept
		 : leftExpr(std::move(left)), rightExpr(std::move(right))
		{ }

		/** @copybrief */
		~RegExAltern() = default;
		/** @} */

		/** @{ */
		void Traverse(RegExTraverser& traverser) const override;
		/** @} */

		/** @{ */
		/** @brief Get the left subexpression of the RegExAltern. */
		const RegExNode& GetLeftExpr() const noexcept {
			return *leftExpr;
		}

		/** @brief Get the right subexpression of the RegExAltern. */
		const RegExNode& GetRightExpr() const noexcept {
			return *rightExpr;
		}
		/** @} */

	private:
		std::unique_ptr<RegExNode> leftExpr, rightExpr;
	};



	/**
	 * @brief Concatenation expression.
	 */
	class RegExConcat : public RegExNode {
	public:
		/** @{ */
		/** @brief Construct a RegExConcat from its two subexpressions. */
		RegExConcat(std::unique_ptr<RegExNode> left, std::unique_ptr<RegExNode> right) noexcept
		 : leftExpr(std::move(left)), rightExpr(std::move(right))
		{ }

		/** @copybrief */
		~RegExConcat() = default;
		/** @} */

		/** @{ */
		void Traverse(RegExTraverser& traverser) const override;
		/** @} */

		/** @{ */
		/** @brief Get the left subexpression of the RegExConcat. */
		const RegExNode& GetLeftExpr() const noexcept {
			return *leftExpr;
		}

		/** @brief Get the right subexpression of the RegExConcat. */
		const RegExNode& GetRightExpr() const noexcept {
			return *rightExpr;
		}
		/** @} */

	private:
		std::unique_ptr<RegExNode> leftExpr, rightExpr;
	};



	/**
	 * @brief Provides a mechanism for traversing regular expressions.
	 */
	class RegExTraverser {
	public:
		/** @{ */
		virtual void Visit(const RegExLiteral& literal) = 0;
		virtual void Visit(const RegExStar& star) = 0;
		virtual void Visit(const RegExAltern& altern) = 0;
		virtual void Visit(const RegExConcat& concat) = 0;
		/** @} */

	protected:
		/** @{ */
		/** @copybrief */
		RegExTraverser() = default;

		/** @copybrief */
		~RegExTraverser() = default;
		/** @} */

		/** @{ */
		RegExTraverser(const RegExTraverser&) = default;
		RegExTraverser& operator=(const RegExTraverser&) = default;
		/** @} */

		/** @{ */
		RegExTraverser(RegExTraverser&&) = default;
		RegExTraverser& operator=(RegExTraverser&&) = default;
		/** @} */
	};
}

#endif