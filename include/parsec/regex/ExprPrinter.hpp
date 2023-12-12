#ifndef PARSEC_REGEX_EXPR_PRINTER_HEADER
#define PARSEC_REGEX_EXPR_PRINTER_HEADER

#include "ExprNode.hpp"
#include "NodeVisitor.hpp"

#include <iostream>

namespace parsec::regex {

	/**
	 * Prints out regular expressions.
	 */
	class ExprPrinter : NodeVisitor {
	public:
		
		explicit ExprPrinter(std::ostream& out = std::cout) noexcept
			: m_out(&out)
		{ }


		/** @{ */
		ExprPrinter(ExprPrinter&&) = default;
		ExprPrinter& operator=(ExprPrinter&&) = default;

		ExprPrinter(const ExprPrinter&) = delete;
		ExprPrinter& operator=(const ExprPrinter&) = delete;
		/** @} */


		/** @{ */
		void print(const ExprNode& n) {
			n.acceptVisitor(*this);
		}
		/** @} */


	private:
		
		void visit(const CharAtom& n) override;
		void visit(const NilExpr& n) override;

		void visit(const OptionalExpr& n) override;
		void visit(const PlusExpr& n) override;
		void visit(const StarExpr& n) override;

		void visit(const AlternExpr& n) override;
		void visit(const ConcatExpr& n) override;


		std::ostream* m_out = {};

	};

}

#endif