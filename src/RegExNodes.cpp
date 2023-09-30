#include "RegExNodes.hpp"
#include "RegExPrinter.hpp"

namespace parsec {
	void RegExNode::Print(std::ostream& out) const {
		RegExPrinter printer(out);
		AcceptVisitor(printer);
	}

	void RegExLiteral::AcceptVisitor(RegExVisitor& visitor) const {
		visitor.Visit(*this);
	}

	void RegExStar::AcceptVisitor(RegExVisitor& visitor) const {
		visitor.Visit(*this);
	}
	
	void RegExAltern::AcceptVisitor(RegExVisitor& visitor) const {
		visitor.Visit(*this);
	}

	void RegExConcat::AcceptVisitor(RegExVisitor& visitor) const {
		visitor.Visit(*this);
	}
}