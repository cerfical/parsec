#ifndef PARSEC_FG_RULES_RULE_PRINTER_HEADER
#define PARSEC_FG_RULES_RULE_PRINTER_HEADER

#include "RuleNode.hpp"
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
		void print(const RuleNode& n) {
			n.traverseWith(*this);
		}
		/** @} */


	private:
		/** @{ */
		void visitNode(const CharLiteral& n) override;
		void visitNode(const RuleRef& n) override;
		void visitNode(const NilRule& n) override;

		void visitNode(const OptRule& n) override;
		void visitNode(const PlusRule& n) override;
		void visitNode(const StarRule& n) override;

		void visitNode(const RuleAltern& n) override;
		void visitNode(const RuleConcat& n) override;
		/** @} */


		/** @{ */
		std::ostream* m_out = nullptr;
		/** @} */
	};
}

#endif