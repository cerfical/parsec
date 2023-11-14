#include "fg/rules.hpp"

namespace parsec::fg {
	void AtomVisitor::visit(const NilRule& n) {
		// nothing to do
	}

	void AtomVisitor::visit(const PlusRule& n) {
		n.inner()->acceptVisitor(*this);
	}

	void AtomVisitor::visit(const StarRule& n) {
		n.inner()->acceptVisitor(*this);
	}

	void AtomVisitor::visit(const OptionalRule& n) {
		n.inner()->acceptVisitor(*this);
	}

	void AtomVisitor::visit(const RuleAltern& n) {
		n.left()->acceptVisitor(*this);
		n.right()->acceptVisitor(*this);
	}

	void AtomVisitor::visit(const RuleConcat& n) {
		n.left()->acceptVisitor(*this);
		n.right()->acceptVisitor(*this);
	}
}