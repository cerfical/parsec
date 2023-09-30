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
		/** @name Object destruction: */

		/** @{ */
		virtual ~RegExNode() = default;
		/** @} */

		/** @{ */
		/** @brief Performs a double dispatch on the specified visitor. */
		virtual void AcceptVisitor(RegExVisitor& visitor) const = 0;
		
		/** @brief Prints out the expression to the stream. */
		void Print(std::ostream& out = std::cout) const;
		/** @} */

	protected:
		/** @name Object construction: */

		/** @{ */
		RegExNode() = default;
		/** @} */

	private:
		RegExNode(const RegExNode&) = delete;
		RegExNode& operator=(const RegExNode&) = delete;

		RegExNode(RegExNode&&) = delete;
		RegExNode& operator=(RegExNode&&) = delete;
	};



	/**
	 * @brief Literal regular expression.
	 */
	class RegExLiteral : public RegExNode {
	public:
		/** @name Object construction/destruction: */

		/** @{ */
		/** @brief Constructs a literal with the specified value. */
		explicit RegExLiteral(char value) noexcept
		 : value(value)
		{ }

		~RegExLiteral() = default;
		/** @} */

		/** @{ */
		void AcceptVisitor(RegExVisitor& visitor) const override;
		
		/** @brief Returns the value of the literal. */
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
		/** @name Object construction/destruction: */

		/** @{ */
		/** @brief Constructs a star with the specified inner expression. */
		explicit RegExStar(std::unique_ptr<RegExNode> expr) noexcept
		 : innerExpr(std::move(expr))
		{ }

		~RegExStar() = default;
		/** @} */

		/** @{ */
		void AcceptVisitor(RegExVisitor& visitor) const override;

		/** @brief Returns the star's inner expression. */
		const RegExNode &GetInnerExpr() const noexcept {
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
		/** @name Object construction/destruction: */

		/** @{ */
		/** @brief Constructs an alternation from two subexpressions. */
		RegExAltern(std::unique_ptr<RegExNode> left, std::unique_ptr<RegExNode> right) noexcept
		 : leftExpr(std::move(left)), rightExpr(std::move(right))
		{ }

		~RegExAltern() = default;
		/** @} */

		/** @{ */
		void AcceptVisitor(RegExVisitor& visitor) const override;

		/** @brief Returns the alternation's left subexpression. */
		const RegExNode &GetLeftExpr() const noexcept {
			return *leftExpr;
		}
		/** @brief Returns the alternation's right subexpression. */
		const RegExNode &GetRightExpr() const noexcept {
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
		/** @name Object construction/destruction: */

		/** @{ */
		/** @brief Constructs a concatenation from two subexpressions. */
		RegExConcat(std::unique_ptr<RegExNode> left, std::unique_ptr<RegExNode> right) noexcept
		 : leftExpr(std::move(left)), rightExpr(std::move(right))
		{ }

		~RegExConcat() = default;
		/** @} */

		/** @{ */
		/** @brief Performs a single dispatch on the specified visitor. */
		void AcceptVisitor(RegExVisitor& visitor) const override;

		/** @brief Returns the concatenation's left subexpression. */
		const RegExNode &GetLeftExpr() const noexcept {
			return *leftExpr;
		}
		/** @brief Returns the concatenation's right subexpression. */
		const RegExNode &GetRightExpr() const noexcept {
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
		/** @name Object construction/destruction: */

		/** @{ */
		RegExVisitor() = default;
		~RegExVisitor() = default;
		/** @} */

		/** @name Copy operations: */

		/** @{ */
		RegExVisitor(const RegExVisitor&) = default;
		RegExVisitor& operator=(const RegExVisitor&) = default;
		/** @} */

		/** @name Move operations: */

		/** @{ */
		RegExVisitor(RegExVisitor&&) = default;
		RegExVisitor& operator=(RegExVisitor&&) = default;
		/** @} */
	};
}

#endif