#include "fg/rules/rules.hpp"

namespace parsec::fg::rules {
	void RulePrinter::visitNode(const CharLiteral& n) {
		*m_out << '(' << n.value() << ')';
	}

	void RulePrinter::visitNode(const RuleRef& n) {
		*m_out << '(' << n.ruleName() << ')';
	}

	void RulePrinter::visitNode(const NilRule&) {
		*m_out << "()";
	}


	void RulePrinter::visitNode(const OptRule& n) {
		n.inner()->traverseWith(*this);
		*m_out << '?';
	}

	void RulePrinter::visitNode(const PlusRule& n) {
		n.inner()->traverseWith(*this);
		*m_out << '+';
	}

	void RulePrinter::visitNode(const StarRule& n) {
		n.inner()->traverseWith(*this);
		*m_out << '*';
	}


	void RulePrinter::visitNode(const RuleAltern& n) {
		*m_out << '(';
		n.left()->traverseWith(*this);
		*m_out << '|';
		n.right()->traverseWith(*this);
		*m_out << ')';
	}

	void RulePrinter::visitNode(const RuleConcat& n) {
		*m_out << '(';
		n.left()->traverseWith(*this);
		n.right()->traverseWith(*this);
		*m_out << ')';
	}
}