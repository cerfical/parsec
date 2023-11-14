#include "fg/rules.hpp"
#include "utils/chars.hpp"

namespace parsec::fg {
	void RulePrinter::visit(const Atom& n) {
		*m_out << '(';
		for(const auto ch : n.value()) {
			*m_out << escapeChar(ch);
		}
		*m_out << ')';
	}

	void RulePrinter::visit(const NilRule&) {
		*m_out << "()";
	}


	void RulePrinter::visit(const OptionalRule& n) {
		n.inner()->acceptVisitor(*this);
		*m_out << '?';
	}

	void RulePrinter::visit(const PlusRule& n) {
		n.inner()->acceptVisitor(*this);
		*m_out << '+';
	}

	void RulePrinter::visit(const StarRule& n) {
		n.inner()->acceptVisitor(*this);
		*m_out << '*';
	}


	void RulePrinter::visit(const RuleAltern& n) {
		*m_out << '(';
		n.left()->acceptVisitor(*this);
		*m_out << '|';
		n.right()->acceptVisitor(*this);
		*m_out << ')';
	}

	void RulePrinter::visit(const RuleConcat& n) {
		*m_out << '(';
		n.left()->acceptVisitor(*this);
		n.right()->acceptVisitor(*this);
		*m_out << ')';
	}
}