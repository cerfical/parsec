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
		/** @brief Construct a new RegExPrinter that prints to the @c std::cout. */
		RegExPrinter() = default;

		/** @brief Construct a new RegExPrinter that prints to a @c std::ostream. */
		explicit RegExPrinter(std::ostream* out) noexcept
		 : out(out)
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
		/** @brief Print a RegExChar node. */
		void VisitNode(const RegExChar& n) override;

		/** @brief Print a RegExNil node. */
		void VisitNode(const RegExNil& n) override;

		/** @brief Print a RegExOpt node. */
		void VisitNode(const RegExOpt& n) override;

		/** @brief Print a RegExPlus node. */
		void VisitNode(const RegExPlus& n) override;

		/** @brief Print a RegExStar node. */
		void VisitNode(const RegExStar& n) override;
		
		/** @brief Print a RegExAltern node. */
		void VisitNode(const RegExAltern& n) override;
		
		/** @brief Print a RegExConcat node. */
		void VisitNode(const RegExConcat& n) override;
		/** @} */

	private:		
		std::ostream* out = &std::cout;
	};
}

#endif