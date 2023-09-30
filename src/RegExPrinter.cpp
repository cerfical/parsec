#include "RegExPrinter.hpp"

namespace parsec {
	void RegExPrinter::Visit(const RegExChar& ch) {
		*out << ch.GetValue();
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