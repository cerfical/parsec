#include "RegExPrinter.hpp"

namespace parsec {
	void RegExPrinter::VisitNode(const RegExChar& n) {
		*out << n.GetValue();
	}

	void RegExPrinter::VisitNode(const RegExNil& n) {
		*out << "()";
	}
	
	void RegExPrinter::VisitNode(const RegExOpt& n) {
		*out << '(';
		n.GetChild().TraverseWith(*this);
		*out << "?)";
	}

	void RegExPrinter::VisitNode(const RegExPlus& n) {
		*out << '(';
		n.GetChild().TraverseWith(*this);
		*out << "+)";
	}

	void RegExPrinter::VisitNode(const RegExStar& n) {
		*out << '(';
		n.GetChild().TraverseWith(*this);
		*out << "*)";
	}

	void RegExPrinter::VisitNode(const RegExAltern& n) {
		*out << '(';
		n.GetLeftChild().TraverseWith(*this);
		*out << '|';
		n.GetRightChild().TraverseWith(*this);
		*out << ')';
	}

	void RegExPrinter::VisitNode(const RegExConcat& n) {
		*out << '(';
		n.GetLeftChild().TraverseWith(*this);
		n.GetRightChild().TraverseWith(*this);
		*out << ')';
	}
}