#ifndef PARSEC_FG_ATOM_VISITOR_HEADER
#define PARSEC_FG_ATOM_VISITOR_HEADER

#include "RuleTraverser.hpp"

namespace parsec::fg {
	class AtomVisitor : public RuleTraverser {
	public:
		/** @{ */
		void visit(const NilRule& n) override;
		void visit(const PlusRule& n) override;
		void visit(const StarRule& n) override;
		void visit(const OptionalRule& n) override;
		void visit(const RuleAltern& n) override;
		void visit(const RuleConcat& n) override;
		/** @} */

	protected:
		/** @{ */
		~AtomVisitor() = default;
		/** @} */
	};
}

#endif