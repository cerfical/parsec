#ifndef PARSEC_FG_RULES_RULE_TRAVERSER_HEADER
#define PARSEC_FG_RULES_RULE_TRAVERSER_HEADER

namespace parsec::fg::rules {
	class CharLiteral;
	class RuleRef;
	class NilRule;
	
	class PlusRule;
	class StarRule;
	class OptRule;

	class RuleAltern;
	class RuleConcat;


	class RuleTraverser {
	public:
		/** @{ */
		virtual void visitNode(const CharLiteral& n) = 0;
		virtual void visitNode(const RuleRef& n) = 0;
		virtual void visitNode(const NilRule& n) = 0;

		virtual void visitNode(const PlusRule& n) = 0;
		virtual void visitNode(const StarRule& n) = 0;
		virtual void visitNode(const OptRule& n) = 0;

		virtual void visitNode(const RuleAltern& n) = 0;
		virtual void visitNode(const RuleConcat& n) = 0;
		/** @} */


	protected:
		/** @{ */
		~RuleTraverser() = default;
		/** @} */
	};
}

#endif