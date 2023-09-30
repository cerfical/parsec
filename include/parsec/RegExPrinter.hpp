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
		/** @{ */
		/** @brief Construct a new RegExPrinter that prints to a @c std::ostream. */
		explicit RegExPrinter(std::ostream& out = std::cout) noexcept
		 : out(&out)
		{ }

		/** @brief Destroy the RegExPrinter. */
		~RegExPrinter() = default;
		/** @} */

		/** @{ */
		/** @brief Construct a new RegExPrinter by moving from another RegExPrinter. */
		RegExPrinter(RegExPrinter&&) = default;

		/** @brief Move another RegExPrinter into the RegExPrinter. */
		RegExPrinter& operator=(RegExPrinter&&) = default;
		/** @} */

		/** @{ */
		RegExPrinter(const RegExPrinter&) = delete;
		RegExPrinter& operator=(const RegExPrinter&) = delete;
		/** @} */

		/** @{ */
		/** @brief Print a literal. */
		void Visit(const RegExLiteral& literal) override;

		/** @brief Print a star and its inner expression. */
		void Visit(const RegExStar& star) override;
		
		/** @brief Print an alternation and its subexpressions. */
		void Visit(const RegExAltern& altern) override;
		
		/** @brief Print a concatenation and its subexpressions. */
		void Visit(const RegExConcat& concat) override;
		/** @} */

	private:		
		std::ostream* out = &std::cout;
	};
}

#endif