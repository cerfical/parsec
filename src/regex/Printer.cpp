#include "regex/Printer.hpp"
#include "regex/nodes.hpp"

namespace parsec::regex {
	void Printer::visitNode(const CharLiteral& n) {
		*m_out << n.value();
	}

	void Printer::visitNode(const NilExpr&) {
		*m_out << "()";
	}

	
	void Printer::visitNode(const OptionalExpr& n) {
		n.inner()->traverseWith(*this);
		*m_out << '?';
	}

	void Printer::visitNode(const PlusExpr& n) {
		n.inner()->traverseWith(*this);
		*m_out << '+';
	}

	void Printer::visitNode(const StarExpr& n) {
		n.inner()->traverseWith(*this);
		*m_out << '*';
	}


	void Printer::visitNode(const AlternExpr& n) {
		*m_out << '(';
		n.left()->traverseWith(*this);
		*m_out << '|';
		n.right()->traverseWith(*this);
		*m_out << ')';
	}

	void Printer::visitNode(const ConcatExpr& n) {
		*m_out << '(';
		n.left()->traverseWith(*this);
		n.right()->traverseWith(*this);
		*m_out << ')';
	}
}