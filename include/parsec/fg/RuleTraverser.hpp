#ifndef PARSEC_FG_RULE_TRAVERSER_HEADER
#define PARSEC_FG_RULE_TRAVERSER_HEADER

#include "RuleVisitor.hpp"
#include "Rule.hpp"

namespace parsec::fg {
	class RuleTraverser : RuleVisitor {
	protected:
		/** @{ */
		~RuleTraverser() = default;
		/** @} */


		/** @{ */
		void traverse(const Rule& n) {
			n.acceptVisitor(*this);
		}
		/** @} */


		/** @{ */
		void visit(const Atom& n) override;
		void visit(const NilRule& n) override;

		void visit(const PlusRule& n) override;
		void visit(const StarRule& n) override;
		void visit(const OptionalRule& n) override;
		
		void visit(const RuleAltern& n) override;
		void visit(const RuleConcat& n) override;
		/** @} */
	};
}

#endif