#include "regex/ExprTraverser.hpp"
#include "regex/nodes.hpp"

namespace parsec::regex {
	using namespace nodes;

	void ExprTraverser::visit(const CharAtom& n) {
		// nothing to do
	}

	void ExprTraverser::visit(const NilExpr& n) {
		// nothing to do
	}


	void ExprTraverser::visit(const PlusExpr& n) {
		n.inner()->acceptVisitor(*this);
	}

	void ExprTraverser::visit(const StarExpr& n) {
		n.inner()->acceptVisitor(*this);
	}

	void ExprTraverser::visit(const OptionalExpr& n) {
		n.inner()->acceptVisitor(*this);
	}


	void ExprTraverser::visit(const AlternExpr& n) {
		n.left()->acceptVisitor(*this);
		n.right()->acceptVisitor(*this);
	}

	void ExprTraverser::visit(const ConcatExpr& n) {
		n.left()->acceptVisitor(*this);
		n.right()->acceptVisitor(*this);
	}
}