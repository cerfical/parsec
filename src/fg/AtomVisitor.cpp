#include "fg/rules.hpp"

namespace parsec::fg {
	void AtomVisitor::visit(const NilRule& n) {
		// nothing to do
	}

	void AtomVisitor::visit(const PlusRule& n) {
		n.inner()->traverse(*this);
	}

	void AtomVisitor::visit(const StarRule& n) {
		n.inner()->traverse(*this);
	}

	void AtomVisitor::visit(const OptionalRule& n) {
		n.inner()->traverse(*this);
	}

	void AtomVisitor::visit(const RuleAltern& n) {
		n.left()->traverse(*this);
		n.right()->traverse(*this);
	}

	void AtomVisitor::visit(const RuleConcat& n) {
		n.left()->traverse(*this);
		n.right()->traverse(*this);
	}
}