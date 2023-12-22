#ifndef PARSEC_REGEX_EXPR_TRAVERSER_HEADER
#define PARSEC_REGEX_EXPR_TRAVERSER_HEADER

#include "nodes/NodeVisitor.hpp"
#include "nodes/ExprNode.hpp"

namespace parsec::regex {

	/**
	 * @brief Traverses an ExprNode by visiting each subexpression it consists of.
	 */
	class ExprTraverser : nodes::NodeVisitor {
	public:

		void traverse(const nodes::ExprNode& n) {
			n.acceptVisitor(*this);
		}

	protected:
		
		~ExprTraverser() = default;


		void visit(const nodes::CharAtom& n) override;
		void visit(const nodes::NilExpr& n) override;

		void visit(const nodes::PlusExpr& n) override;
		void visit(const nodes::StarExpr& n) override;
		void visit(const nodes::OptionalExpr& n) override;
		
		void visit(const nodes::AlternExpr& n) override;
		void visit(const nodes::ConcatExpr& n) override;

	};

}

#endif