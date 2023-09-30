#include "RegExPrinter.hpp"

namespace parsec {
	void RegExPrinter::VisitNode(const RegExChar& n) {
		*m_out << n.GetValue();
	}

	void RegExPrinter::VisitNode(const RegExNil& n) {
		*m_out << "()";
	}
	
	void RegExPrinter::VisitNode(const RegExOpt& n) {
		*m_out << '(';
		n.GetInnerExpr()->TraverseWith(*this);
		*m_out << "?)";
	}

	void RegExPrinter::VisitNode(const RegExPlus& n) {
		*m_out << '(';
		n.GetInnerExpr()->TraverseWith(*this);
		*m_out << "+)";
	}

	void RegExPrinter::VisitNode(const RegExStar& n) {
		*m_out << '(';
		n.GetInnerExpr()->TraverseWith(*this);
		*m_out << "*)";
	}

	void RegExPrinter::VisitNode(const RegExAltern& n) {
		*m_out << '(';
		n.GetLeftExpr()->TraverseWith(*this);
		*m_out << '|';
		n.GetRightExpr()->TraverseWith(*this);
		*m_out << ')';
	}

	void RegExPrinter::VisitNode(const RegExConcat& n) {
		*m_out << '(';
		n.GetLeftExpr()->TraverseWith(*this);
		n.GetRightExpr()->TraverseWith(*this);
		*m_out << ')';
	}
}