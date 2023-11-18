#ifndef PARSEC_REGEX_NODE_VISITOR_HEADER
#define PARSEC_REGEX_NODE_VISITOR_HEADER

namespace parsec::regex {
	class CharAtom;
	class NilExpr;
	
	class PlusExpr;
	class StarExpr;
	class OptionalExpr;

	class AlternExpr;
	class ConcatExpr;


	class NodeVisitor {
	public:
		/** @{ */
		virtual void visit(const CharAtom& n) = 0;
		virtual void visit(const NilExpr& n) = 0;

		virtual void visit(const PlusExpr& n) = 0;
		virtual void visit(const StarExpr& n) = 0;
		virtual void visit(const OptionalExpr& n) = 0;

		virtual void visit(const AlternExpr& n) = 0;
		virtual void visit(const ConcatExpr& n) = 0;
		/** @} */

	protected:
		/** @{ */
		~NodeVisitor() = default;
		/** @} */
	};
}

#endif