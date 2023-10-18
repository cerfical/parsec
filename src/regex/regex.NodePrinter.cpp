#include "regex/NodePrinter.hpp"
#include "regex/nodes.hpp"

namespace parsec::regex {
	void NodePrinter::visitNode(const CharLiteral& n) {
		*m_out << n.value();
	}

	void NodePrinter::visitNode(const NilExpr&) {
		*m_out << "()";
	}

	
	void NodePrinter::visitNode(const OptionalExpr& n) {
		n.inner()->traverseWith(*this);
		*m_out << '?';
	}

	void NodePrinter::visitNode(const PlusExpr& n) {
		n.inner()->traverseWith(*this);
		*m_out << '+';
	}

	void NodePrinter::visitNode(const StarExpr& n) {
		n.inner()->traverseWith(*this);
		*m_out << '*';
	}


	void NodePrinter::visitNode(const AlternExpr& n) {
		*m_out << '(';
		n.left()->traverseWith(*this);
		*m_out << '|';
		n.right()->traverseWith(*this);
		*m_out << ')';
	}

	void NodePrinter::visitNode(const ConcatExpr& n) {
		*m_out << '(';
		n.left()->traverseWith(*this);
		n.right()->traverseWith(*this);
		*m_out << ')';
	}
}