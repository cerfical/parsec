#ifndef PARSEC_REGEX_PRINTER_HEADER
#define PARSEC_REGEX_PRINTER_HEADER

#include "RegExNodes.hpp"
#include <iostream>

namespace parsec {
	/**
	 * @brief Prints regular expressions.
	 */
	class RegExPrinter : public RegExVisitor {
	public:
		/** @name Object construction/destruction: */
		
		/** @{ */
		/** @brief Constructs a printer that sends its output to the stream. */
		explicit RegExPrinter(std::ostream& out = std::cout) noexcept
		 : out(&out)
		{ }

		~RegExPrinter() = default;
		/** @} */

		/** @name Move operations: */

		/** @{ */
		RegExPrinter(RegExPrinter&&) = default;
		RegExPrinter& operator=(RegExPrinter&&) = default;
		/** @} */

		/** @{ */
		/** @brief Prints out a literal. */
		void Visit(const RegExLiteral& literal) override;

		/** @brief Prints out a star and its subexpression. */
		void Visit(const RegExStar& star) override;
		
		/** @brief Prints out an alternation and its subexpressions. */
		void Visit(const RegExAltern& altern) override;
		
		/** @brief Prints out a concatenation and its subexpressions. */
		void Visit(const RegExConcat& concat) override;
		/** @} */

	private:
		RegExPrinter(const RegExPrinter&) = delete;
		RegExPrinter& operator=(const RegExPrinter&) = delete;
		
		std::ostream* out = &std::cout;
	};
}

#endif