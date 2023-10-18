#ifndef PARSEC_REGEX_PRINTER_HEADER
#define PARSEC_REGEX_PRINTER_HEADER

#include "ExprNode.hpp"
#include "NodeTraverser.hpp"

#include <iostream>

namespace parsec::regex {
	/**
	 * @brief Prints regular expressions.
	 */
	class Printer : public NodeTraverser {
	public:
		/** @{ */
		/** @brief Construct a new printer that prints to a @c std::ostream. */
		explicit Printer(std::ostream& out = std::cout) noexcept
			: m_out(&out)
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
		/** @brief Prints out a regular expression recursively. */
		void print(const ExprNode& n) {
			n.traverseWith(*this);
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


		/** @{ */
		std::ostream* m_out = &std::cout;
		/** @} */
	};
}

#endif