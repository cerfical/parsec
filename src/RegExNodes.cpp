#include "RegExNodes.hpp"
#include "RegExPrinter.hpp"

namespace {
	using namespace parsec;

	template <typename Node>
	concept IsUnaryExpr = requires(const Node& n) { n.GetInnerExpr(); };

	template <typename Node>
	concept IsBinaryExpr = requires(const Node& n) { n.GetLeftExpr(); n.GetRightExpr(); };

	template <typename Node>
	void TraverseNode(const Node& n, RegExTraverser& traverser) {
		switch(traverser.GetTraversalType()) {
			case TraversalTypes::Preorder: {
				traverser.Visit(n);
				if constexpr(IsBinaryExpr<Node>) {
					n.GetLeftExpr().Traverse(traverser);
					n.GetRightExpr().Traverse(traverser);
				} else if constexpr(IsUnaryExpr<Node>) {
					n.GetInnerExpr().Traverse(traverser);
				} 
				break;
			}
			case TraversalTypes::Postorder: {
				if constexpr(IsBinaryExpr<Node>) {
					n.GetLeftExpr().Traverse(traverser);
					n.GetRightExpr().Traverse(traverser);
				} else if constexpr(IsUnaryExpr<Node>) {
					n.GetInnerExpr().Traverse(traverser);
				} 
				traverser.Visit(n);
				break;
			}
			case TraversalTypes::None: {
				traverser.Visit(n);
				break;
			}
		}
	}
}

namespace parsec {
	void RegExChar::Traverse(RegExTraverser& traverser) const {
		TraverseNode(*this, traverser);
	}

	void RegExStar::Traverse(RegExTraverser& traverser) const {
		TraverseNode(*this, traverser);
	}
	
	void RegExAltern::Traverse(RegExTraverser& traverser) const {
		TraverseNode(*this, traverser);
	}

	void RegExConcat::Traverse(RegExTraverser& traverser) const {
		TraverseNode(*this, traverser);
	}

	std::ostream& operator<<(std::ostream& out, const RegExNode& regex) {
		RegExPrinter printer(out);
		regex.Traverse(printer);
		return out;
	}
}