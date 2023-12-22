#ifndef PARSEC_REGEX_NODES_NODE_TRAVERSER_HEADER
#define PARSEC_REGEX_NODES_NODE_TRAVERSER_HEADER

#include "NodeVisitor.hpp"
#include "ExprNode.hpp"

namespace parsec::regex::nodes {

	/**
	 * @brief Traverses an ExprNode by recursively visiting its child nodes.
	*/
	class NodeTraverser : NodeVisitor {
	public:

		void traverse(const ExprNode& n) {
			n.acceptVisitor(*this);
		}

	protected:
		
		~NodeTraverser() = default;

	private:
		void visit(const PlusExpr& n) override;
		void visit(const StarExpr& n) override;
		void visit(const OptionalExpr& n) override;
		
		void visit(const AlternExpr& n) override;
		void visit(const ConcatExpr& n) override;
	};

}

#endif