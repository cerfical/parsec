#include "RegExPrinter.hpp"
#include "RegExNodes.hpp"

namespace parsec {
	void RegExPrinter::Visit(const RegExLiteral& literal) {
		*out << literal.GetValue();
	}

	void RegExPrinter::Visit(const RegExStar& star) {
		*out << '(';
		star.GetInnerExpr().AcceptVisitor(*this);
		*out << "*)";
	}

	void RegExPrinter::Visit(const RegExAltern& altern) {
		*out << '(';
		altern.GetLeftExpr().AcceptVisitor(*this);
		*out << '|';
		altern.GetRightExpr().AcceptVisitor(*this);
		*out << ')';
	}

	void RegExPrinter::Visit(const RegExConcat& concat) {
		*out << '(';
		concat.GetLeftExpr().AcceptVisitor(*this);
		concat.GetRightExpr().AcceptVisitor(*this);
		*out << ')';
	}
}