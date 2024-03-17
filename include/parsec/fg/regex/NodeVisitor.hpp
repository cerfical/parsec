#ifndef PARSEC_FG_REGEX_NODE_VISITOR_HEADER
#define PARSEC_FG_REGEX_NODE_VISITOR_HEADER

namespace parsec::fg::regex {

	class SymbolAtom;

	class StarClosure;
	class PlusClosure;
	class OptionalExpr;
	
	class AlternExpr;
	class ConcatExpr;

	class NodeVisitor {
	public:

		virtual void visit(const SymbolAtom& n) = 0;

		virtual void visit(const StarClosure& n) = 0;
		virtual void visit(const PlusClosure& n) = 0;
		virtual void visit(const OptionalExpr& n) = 0;
		
		virtual void visit(const AlternExpr& n) = 0;
		virtual void visit(const ConcatExpr& n) = 0;

	protected:
		~NodeVisitor() = default;
	};

}

#endif