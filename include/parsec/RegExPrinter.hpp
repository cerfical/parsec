#ifndef PARSEC_REG_EX_PRINTER_HEADER
#define PARSEC_REG_EX_PRINTER_HEADER

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
		 : m_out(out)
		{ }
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
		/** @brief Print out a single regular expression. */
		void Print(const RegExNode& regex) {
			regex.TraverseWith(*this);
		}
		/** @} */

	private:
		/** @{ */
		void VisitNode(const RegExChar& n) override;
		void VisitNode(const RegExNil& n) override;
		void VisitNode(const RegExOpt& n) override;
		void VisitNode(const RegExPlus& n) override;
		void VisitNode(const RegExStar& n) override;
		void VisitNode(const RegExAltern& n) override;
		void VisitNode(const RegExConcat& n) override;
		/** @} */

		std::ostream* m_out = &std::cout;
	};
}

#endif