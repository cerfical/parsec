#ifndef PARSEC_REGEX_PRINTER_HEADER
#define PARSEC_REGEX_PRINTER_HEADER

#include "RegExNodes.hpp"
#include <iostream>

namespace parsec {
	/**
	 * @brief Prints regular expressions.
	 */
	class RegExPrinter : public RegExTraverser {
	public:
		/** @{ */
		/** @brief Construct a new RegExPrinter that prints to a @c std::ostream. */
		explicit RegExPrinter(std::ostream& out = std::cout) noexcept
		 : out(&out)
		{ }

		/** @copybrief */
		~RegExPrinter() = default;
		/** @} */

		/** @{ */
		RegExPrinter(RegExPrinter&&) = default;
		RegExPrinter& operator=(RegExPrinter&&) = default;
		/** @} */

		/** @{ */
		RegExPrinter(const RegExPrinter&) = delete;
		RegExPrinter& operator=(const RegExPrinter&) = delete;
		/** @} */

		/** @{ */
		/** @brief Print out a character. */
		void VisitNode(const RegExChar& ch) override;

		/** @brief Print out a star expression and its inner expression. */
		void VisitNode(const RegExStar& star) override;
		
		/** @brief Print out an alternation expression and its subexpressions. */
		void VisitNode(const RegExAltern& altern) override;
		
		/** @brief Print out a concatenation expression and its subexpressions. */
		void VisitNode(const RegExConcat& concat) override;
		/** @} */

	private:		
		std::ostream* out = nullptr;
	};
}

#endif