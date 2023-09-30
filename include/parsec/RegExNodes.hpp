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
		/** @brief Destroy RegExNode. */
		virtual ~RegExNode() = default;
		/** @} */

		/** @{ */
		/** @brief Perform a double dispatch on a visitor. */
		virtual void AcceptVisitor(RegExVisitor& visitor) const = 0;
		
		/** @brief Prints out RegExNode to a stream. */
		void Print(std::ostream& out = std::cout) const;
		/** @} */

	protected:
		/** @brief Construct a RegExNode. */
		RegExNode() = default;
	};



	/**
	 * @brief Literal regular expression.
	 */
	class RegExLiteral : public RegExNode {
	public:
		/** @{ */
		/** @brief Construct a RegExLiteral from its value. */
		explicit RegExLiteral(char value) noexcept
		 : value(value)
		{ }

		/** @brief Destroy RegExLiteral. */
		~RegExLiteral() = default;
		/** @} */

		/** @{ */
		void AcceptVisitor(RegExVisitor& visitor) const override;
		
		/** @brief Get value of RegExLiteral. */
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
		/** @brief Construct a RegExStar from its inner expression. */
		explicit RegExStar(std::unique_ptr<RegExNode> expr) noexcept
		 : innerExpr(std::move(expr))
		{ }

		/** @brief Destroy RegExStar. */
		~RegExStar() = default;
		/** @} */

		/** @{ */
		void AcceptVisitor(RegExVisitor& visitor) const override;

		/** @brief Get inner expression of RegExStar. */
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
		/** @brief Construct a RegExAltern from its subexpressions. */
		RegExAltern(std::unique_ptr<RegExNode> left, std::unique_ptr<RegExNode> right) noexcept
		 : leftExpr(std::move(left)), rightExpr(std::move(right))
		{ }

		/** @brief Destroy RegExAltern. */
		~RegExAltern() = default;
		/** @} */

		/** @{ */
		void AcceptVisitor(RegExVisitor& visitor) const override;

		/** @brief Get left subexpression of RegExAltern. */
		const RegExNode& GetLeftExpr() const noexcept {
			return *leftExpr;
		}
		/** @brief Get right subexpression of RegExAltern. */
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

		/** @brief Destroy RegExConcat. */
		~RegExConcat() = default;
		/** @} */

		/** @{ */
		void AcceptVisitor(RegExVisitor& visitor) const override;

		/** @brief Get left subexpression of RegExConcat. */
		const RegExNode& GetLeftExpr() const noexcept {
			return *leftExpr;
		}
		/** @brief Get right subexpression of RegExConcat. */
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
		/** @brief Construct a RegExVisitor. */
		RegExVisitor() = default;

		/** @brief Destroy RegExVisitor. */
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