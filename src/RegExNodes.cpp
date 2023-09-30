#include "RegExNodes.hpp"
#include "RegExPrinter.hpp"

namespace parsec {
	void RegExNode::Print(std::ostream& out) const {
		RegExPrinter printer(out);
		Traverse(printer);
	}

	void RegExLiteral::Traverse(RegExTraverser& traverser) const {
		traverser.Visit(*this);
	}

	void RegExStar::Traverse(RegExTraverser& traverser) const {
		traverser.Visit(*this);
	}
	
	void RegExAltern::Traverse(RegExTraverser& traverser) const {
		traverser.Visit(*this);
	}

	void RegExConcat::Traverse(RegExTraverser& traverser) const {
		traverser.Visit(*this);
	}
}