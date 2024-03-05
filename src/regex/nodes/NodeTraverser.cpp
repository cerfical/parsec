#include "regex/nodes.hpp"

namespace parsec::regex::nodes {
	void NodeTraverser::visit(const CharAtom& n) {
		// nothing to do here
	}

	void NodeTraverser::visit(const NilExpr& n) {
		// nothing to do here
	}

	void NodeTraverser::visit(const PlusExpr& n) {
		n.inner->acceptVisitor(*this);
	}

	void NodeTraverser::visit(const StarExpr& n) {
		n.inner->acceptVisitor(*this);
	}

	void NodeTraverser::visit(const OptionalExpr& n) {
		n.inner->acceptVisitor(*this);
	}

	void NodeTraverser::visit(const AlternExpr& n) {
		n.left->acceptVisitor(*this);
		n.right->acceptVisitor(*this);
	}

	void NodeTraverser::visit(const ConcatExpr& n) {
		n.left->acceptVisitor(*this);
		n.right->acceptVisitor(*this);
	}
}