#include "fg/rules.hpp"

namespace parsec::fg {
	void RulePrinter::visit(const Atom& n) {
		*m_out << '(' << n.value() << ')';
	}

	void RulePrinter::visit(const NilRule&) {
		*m_out << "()";
	}


	void RulePrinter::visit(const OptionalRule& n) {
		n.inner()->traverse(*this);
		*m_out << '?';
	}

	void RulePrinter::visit(const PlusRule& n) {
		n.inner()->traverse(*this);
		*m_out << '+';
	}

	void RulePrinter::visit(const StarRule& n) {
		n.inner()->traverse(*this);
		*m_out << '*';
	}


	void RulePrinter::visit(const RuleAltern& n) {
		*m_out << '(';
		n.left()->traverse(*this);
		*m_out << '|';
		n.right()->traverse(*this);
		*m_out << ')';
	}

	void RulePrinter::visit(const RuleConcat& n) {
		*m_out << '(';
		n.left()->traverse(*this);
		n.right()->traverse(*this);
		*m_out << ')';
	}
}