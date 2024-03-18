#ifndef PARSEC_PARS_AST_NODE_VISITOR_HEADER
#define PARSEC_PARS_AST_NODE_VISITOR_HEADER

namespace parsec::pars::ast {

	class NilNode;
	class NodeList;

	class InlineToken;
	class NamedToken;
	
	class NilRule;
	class NameRule;
	class NamedRule;
	
	class ConcatRule;
	class AlternRule;

	class OptionalRule;
	class PlusRule;
	class StarRule;

	class NodeVisitor {
	public:

		virtual void visit(const NilNode& n) = 0;
		virtual void visit(const NodeList& n) = 0;
		
		virtual void visit(const InlineToken& n) = 0;
		virtual void visit(const NamedToken& n) = 0;
		
		virtual void visit(const NilRule& n) = 0;
		virtual void visit(const NameRule& n) = 0;
		virtual void visit(const NamedRule& n) = 0;
	
		virtual void visit(const ConcatRule& n) = 0;
		virtual void visit(const AlternRule& n) = 0;

		virtual void visit(const OptionalRule& n) = 0;
		virtual void visit(const PlusRule& n) = 0;
		virtual void visit(const StarRule& n) = 0;

	protected:
		~NodeVisitor() = default;
	};

}

#endif