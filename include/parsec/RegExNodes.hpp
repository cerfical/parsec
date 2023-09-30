#ifndef PARSEC_REGEX_NODES_HEADER
#define PARSEC_REGEX_NODES_HEADER

#include <iostream>
#include <memory>

namespace parsec {
	class RegExVisitor;
	/**
	 * @brief Common interface for all regular expression nodes.
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
		/** @brief Destroy the RegExNode. */
		virtual ~RegExNode() = default;
		/** @} */

		/** @{ */
		/** @brief Perform a double dispatch on the RegExNode. */
		virtual void AcceptVisitor(RegExVisitor& visitor) const = 0;
		
		/** @brief Print the RegExNode to a @c std::ostream. */
		void Print(std::ostream& out = std::cout) const;
		/** @} */

	protected:
		/** @brief Construct a new RegExNode. */
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

		/** @brief Destroy the RegExLiteral. */
		~RegExLiteral() = default;
		/** @} */

		/** @{ */
		void AcceptVisitor(RegExVisitor& visitor) const override;
		
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

		/** @brief Destroy the RegExStar. */
		~RegExStar() = default;
		/** @} */

		/** @{ */
		void AcceptVisitor(RegExVisitor& visitor) const override;

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

		/** @brief Destroy the RegExAltern. */
		~RegExAltern() = default;
		/** @} */

		/** @{ */
		void AcceptVisitor(RegExVisitor& visitor) const override;

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

		/** @brief Destroy the RegExConcat. */
		~RegExConcat() = default;
		/** @} */

		/** @{ */
		void AcceptVisitor(RegExVisitor& visitor) const override;

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
	 * @brief Provides a double dispatch mechanism for regular expressions.
	 */
	class RegExVisitor {
	public:
		/** @{ */
		virtual void Visit(const RegExLiteral& literal) = 0;
		virtual void Visit(const RegExStar& star) = 0;
		virtual void Visit(const RegExAltern& altern) = 0;
		virtual void Visit(const RegExConcat& concat) = 0;
		/** @} */

	protected:
		/** @{ */
		/** @brief Construct a new RegExVisitor. */
		RegExVisitor() = default;

		/** @brief Destroy the RegExVisitor. */
		~RegExVisitor() = default;
		/** @} */

		/** @{ */
		RegExVisitor(const RegExVisitor&) = default;
		RegExVisitor& operator=(const RegExVisitor&) = default;
		/** @} */

		/** @{ */
		RegExVisitor(RegExVisitor&&) = default;
		RegExVisitor& operator=(RegExVisitor&&) = default;
		/** @} */
	};
}

#endif