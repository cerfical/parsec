#ifndef PARSEC_FG_RULES_RULE_PRINTER_HEADER
#define PARSEC_FG_RULES_RULE_PRINTER_HEADER

#include "Rule.hpp"
#include "RuleTraverser.hpp"

#include <iostream>

namespace parsec::fg::rules {
	class RulePrinter : public RuleTraverser {
	public:
		/** @{ */
		explicit RulePrinter(std::ostream& out = std::cout) noexcept
			: m_out(&out)
		{ }
		/** @} */


		/** @{ */
		RulePrinter(RulePrinter&&) = default;
		RulePrinter& operator=(RulePrinter&&) = default;
		/** @} */


		/** @{ */
		RulePrinter(const RulePrinter&) = delete;
		RulePrinter& operator=(const RulePrinter&) = delete;
		/** @} */


		/** @{ */
		void print(const Rule& n) {
			n.traverse(*this);
		}
		/** @} */


	private:
		/** @{ */
		void visit(const Atom& n) override;
		void visit(const NilRule& n) override;

		void visit(const OptRule& n) override;
		void visit(const PlusRule& n) override;
		void visit(const StarRule& n) override;

		void visit(const RuleAltern& n) override;
		void visit(const RuleConcat& n) override;
		/** @} */


		/** @{ */
		std::ostream* m_out;
		/** @} */
	};
}

#endif