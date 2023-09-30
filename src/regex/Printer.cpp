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
		*m_out << '(';
		n.innerSubexpr()->traverseWith(*this);
		*m_out << "?)";
	}

	void Printer::visitNode(const PlusExpr& n) {
		*m_out << '(';
		n.innerSubexpr()->traverseWith(*this);
		*m_out << "+)";
	}

	void Printer::visitNode(const StarExpr& n) {
		*m_out << '(';
		n.innerSubexpr()->traverseWith(*this);
		*m_out << "*)";
	}

	void Printer::visitNode(const AlternExpr& n) {
		*m_out << '(';
		n.leftSubexpr()->traverseWith(*this);
		*m_out << '|';
		n.rightSubexpr()->traverseWith(*this);
		*m_out << ')';
	}

	void Printer::visitNode(const ConcatExpr& n) {
		*m_out << '(';
		n.leftSubexpr()->traverseWith(*this);
		n.rightSubexpr()->traverseWith(*this);
		*m_out << ')';
	}
}