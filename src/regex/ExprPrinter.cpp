#include "regex/ExprPrinter.hpp"
#include "regex/nodes.hpp"

#include "utils/Chars.hpp"

namespace parsec::regex {
	void ExprPrinter::visit(const CharAtom& n) {
		*m_out << Chars::escapeChar(n.value());
	}

	void ExprPrinter::visit(const NilExpr&) {
		*m_out << "()";
	}


	void ExprPrinter::visit(const OptionalExpr& n) {
		n.inner()->acceptVisitor(*this);
		*m_out << '?';
	}

	void ExprPrinter::visit(const PlusExpr& n) {
		n.inner()->acceptVisitor(*this);
		*m_out << '+';
	}

	void ExprPrinter::visit(const StarExpr& n) {
		n.inner()->acceptVisitor(*this);
		*m_out << '*';
	}


	void ExprPrinter::visit(const AlternExpr& n) {
		*m_out << '(';
		n.left()->acceptVisitor(*this);
		*m_out << '|';
		n.right()->acceptVisitor(*this);
		*m_out << ')';
	}

	void ExprPrinter::visit(const ConcatExpr& n) {
		*m_out << '(';
		n.left()->acceptVisitor(*this);
		n.right()->acceptVisitor(*this);
		*m_out << ')';
	}
}