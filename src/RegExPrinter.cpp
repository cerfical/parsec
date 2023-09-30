#include "RegExPrinter.hpp"
#include "RegExNodes.hpp"

namespace parsec {
	void RegExPrinter::Visit(const RegExLiteral& literal) {
		*out << literal.GetValue();
	}

	void RegExPrinter::Visit(const RegExStar& star) {
		*out << '(';
		star.GetInnerExpr().Traverse(*this);
		*out << "*)";
	}

	void RegExPrinter::Visit(const RegExAltern& altern) {
		*out << '(';
		altern.GetLeftExpr().Traverse(*this);
		*out << '|';
		altern.GetRightExpr().Traverse(*this);
		*out << ')';
	}

	void RegExPrinter::Visit(const RegExConcat& concat) {
		*out << '(';
		concat.GetLeftExpr().Traverse(*this);
		concat.GetRightExpr().Traverse(*this);
		*out << ')';
	}
}