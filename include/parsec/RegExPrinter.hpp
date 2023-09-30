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
		/** @brief Constructs a RegExPrinter that sends its output to @c std::ostream. */
		explicit RegExPrinter(std::ostream& out = std::cout) noexcept
		 : out(&out)
		{ }

		/** @brief Destroy RegExPrinter. */
		~RegExPrinter() = default;
		/** @} */

		/** @{ */
		/** @brief Construct a RegExPrinter from a moved copy of RegExPrinter object. */
		RegExPrinter(RegExPrinter&&) = default;

		/** @brief Assign a moved copy of RegExPrinter object to RegExPrinter. */
		RegExPrinter& operator=(RegExPrinter&&) = default;
		/** @} */

		/** @{ */
		RegExPrinter(const RegExPrinter&) = delete;
		RegExPrinter& operator=(const RegExPrinter&) = delete;
		/** @} */

		/** @{ */
		/** @brief Prints out a literal. */
		void Visit(const RegExLiteral& literal) override;

		/** @brief Prints out a star and its inner expression. */
		void Visit(const RegExStar& star) override;
		
		/** @brief Prints out an alternation and its subexpressions. */
		void Visit(const RegExAltern& altern) override;
		
		/** @brief Prints out a concatenation and its subexpressions. */
		void Visit(const RegExConcat& concat) override;
		/** @} */

	private:		
		std::ostream* out = &std::cout;
	};
}

#endif