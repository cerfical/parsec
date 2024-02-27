#ifndef PARSEC_PARS_AST_NODE_VISITOR_HEADER
#define PARSEC_PARS_AST_NODE_VISITOR_HEADER

namespace parsec::pars::ast {

	class NilNode;
	class NodeList;

	class InlinePattern;
	class NamedPattern;
	
	class NilRule;
	class NameRule;
	class NamedRule;
	
	class ConcatRule;
	class AlternRule;

	class OptionalRule;
	class PlusRule;
	class StarRule;

	/**
	 * @brief Implements the 'Visitor' design pattern for Node%s.
	*/
	class NodeVisitor {
	public:

		virtual void visit(const NilNode& n) = 0;
		virtual void visit(const NodeList& n) = 0;
		
		virtual void visit(const InlinePattern& n) = 0;
		virtual void visit(const NamedPattern& n) = 0;
		
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