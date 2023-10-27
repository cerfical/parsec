#ifndef PARSEC_FG_RULE_TRAVERSER_HEADER
#define PARSEC_FG_RULE_TRAVERSER_HEADER

namespace parsec::fg {
	class Atom;
	class NilRule;
	
	class PlusRule;
	class StarRule;
	class OptionalRule;

	class RuleAltern;
	class RuleConcat;


	class RuleTraverser {
	public:
		/** @{ */
		virtual void visit(const Atom& n) = 0;
		virtual void visit(const NilRule& n) = 0;

		virtual void visit(const PlusRule& n) = 0;
		virtual void visit(const StarRule& n) = 0;
		virtual void visit(const OptionalRule& n) = 0;

		virtual void visit(const RuleAltern& n) = 0;
		virtual void visit(const RuleConcat& n) = 0;
		/** @} */


	protected:
		/** @{ */
		~RuleTraverser() = default;
		/** @} */
	};
}

#endif