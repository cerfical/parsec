#ifndef PARSEC_REGEX_PRINTER_HEADER
#define PARSEC_REGEX_PRINTER_HEADER

#include "ExprNode.hpp"
#include "Traverser.hpp"

#include <iostream>

namespace parsec::regex {
	/**
	 * @brief Prints regular expressions.
	 */
	class Printer : public Traverser {
	public:
		/** @{ */
		/** @brief Construct a new printer that prints to the @c std::cout. */
		Printer() = default;

		/** @brief Construct a new printer that prints to a @c std::ostream. */
		explicit Printer(std::ostream* out) noexcept
			: m_out(out)
		{ }
		/** @} */


		/** @{ */
		Printer(Printer&&) = default;
		Printer& operator=(Printer&&) = default;
		/** @} */

		/** @{ */
		Printer(const Printer&) = delete;
		Printer& operator=(const Printer&) = delete;
		/** @} */


		/** @{ */
		/** @brief Print out a single regular expression. */
		void print(const ExprNode& regex) {
			regex.traverseWith(*this);
		}
		/** @} */

	private:
		/** @{ */
		void visitNode(const CharLiteral& n) override;
		void visitNode(const NilExpr& n) override;
		void visitNode(const OptionalExpr& n) override;
		void visitNode(const PlusExpr& n) override;
		void visitNode(const StarExpr& n) override;
		void visitNode(const AlternExpr& n) override;
		void visitNode(const ConcatExpr& n) override;
		/** @} */

		std::ostream* m_out = &std::cout;
	};
}

#endif