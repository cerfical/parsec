#include "RegExPrinter.hpp"

namespace parsec {
	void RegExPrinter::VisitNode(const RegExChar& ch) {
		*out << ch.GetValue();
	}

	void RegExPrinter::VisitNode(const RegExStar& star) {
		*out << '(';
		star.GetInnerExpr().TraverseWith(*this);
		*out << "*)";
	}

	void RegExPrinter::VisitNode(const RegExAltern& altern) {
		*out << '(';
		altern.GetLeftExpr().TraverseWith(*this);
		*out << '|';
		altern.GetRightExpr().TraverseWith(*this);
		*out << ')';
	}

	void RegExPrinter::VisitNode(const RegExConcat& concat) {
		*out << '(';
		concat.GetLeftExpr().TraverseWith(*this);
		concat.GetRightExpr().TraverseWith(*this);
		*out << ')';
	}
}