#include "fg/RuleTraverser.hpp"
#include "fg/rules.hpp"

namespace parsec::fg {
	void RuleTraverser::visit(const Atom& n) {
		// nothing to do
	}

	void RuleTraverser::visit(const NilRule& n) {
		// nothing to do
	}


	void RuleTraverser::visit(const PlusRule& n) {
		n.inner()->acceptVisitor(*this);
	}

	void RuleTraverser::visit(const StarRule& n) {
		n.inner()->acceptVisitor(*this);
	}

	void RuleTraverser::visit(const OptionalRule& n) {
		n.inner()->acceptVisitor(*this);
	}


	void RuleTraverser::visit(const RuleAltern& n) {
		n.left()->acceptVisitor(*this);
		n.right()->acceptVisitor(*this);
	}

	void RuleTraverser::visit(const RuleConcat& n) {
		n.left()->acceptVisitor(*this);
		n.right()->acceptVisitor(*this);
	}
}