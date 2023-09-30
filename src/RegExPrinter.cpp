#include "RegExPrinter.hpp"

namespace parsec {
	void RegExPrinter::Visit(const RegExChar& ch) {
		*out << ch.GetValue();
	}

	void RegExPrinter::Visit(const RegExStar& star) {
		*out << '(';
		star.GetInnerExpr().TraverseWith(*this);
		*out << "*)";
	}

	void RegExPrinter::Visit(const RegExAltern& altern) {
		*out << '(';
		altern.GetLeftExpr().TraverseWith(*this);
		*out << '|';
		altern.GetRightExpr().TraverseWith(*this);
		*out << ')';
	}

	void RegExPrinter::Visit(const RegExConcat& concat) {
		*out << '(';
		concat.GetLeftExpr().TraverseWith(*this);
		concat.GetRightExpr().TraverseWith(*this);
		*out << ')';
	}
}