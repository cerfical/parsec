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
					n.GetLeftExpr().TraverseWith(traverser);
					n.GetRightExpr().TraverseWith(traverser);
				} else if constexpr(IsUnaryExpr<Node>) {
					n.GetInnerExpr().TraverseWith(traverser);
				} 
				break;
			}
			case TraversalTypes::Postorder: {
				if constexpr(IsBinaryExpr<Node>) {
					n.GetLeftExpr().TraverseWith(traverser);
					n.GetRightExpr().TraverseWith(traverser);
				} else if constexpr(IsUnaryExpr<Node>) {
					n.GetInnerExpr().TraverseWith(traverser);
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

	class ComputeFirstChars : public RegExTraverser {
	public:
		/** @{ */
		ComputeFirstChars(RegExCharList& chars) noexcept
		 : chars(chars)
		{ }
		/** @} */

		/** @{ */
		void Visit(const RegExChar& ch) override {
			chars.push_back(&ch);
		}
		
		void Visit(const RegExStar& n) override {
			// add the first chars of the inner expression
			n.GetInnerExpr().TraverseWith(*this);			
		}
		
		void Visit(const RegExAltern& n) override {
			// add the first chars of the left child
			n.GetLeftExpr().TraverseWith(*this);
			// add the first chars of the right child
			n.GetRightExpr().TraverseWith(*this);
		}

		void Visit(const RegExConcat& n) override {
			// add the first chars of the left child
			n.GetLeftExpr().TraverseWith(*this);
			if(n.GetLeftExpr().IsNullable()) {
				// add the first chars of the right child
				n.GetRightExpr().TraverseWith(*this);
			}
		}
		/** @} */

	private:
		RegExCharList& chars;
	};

	class ComputeLastChars : public RegExTraverser {
	public:
		/** @{ */
		ComputeLastChars(RegExCharList& chars) noexcept
		 : chars(chars)
		{ }
		/** @} */

		/** @{ */
		void Visit(const RegExChar& ch) override {
			chars.push_back(&ch);
		}
		
		void Visit(const RegExStar& n) override {
			// add the last chars of the inner expression
			n.GetInnerExpr().TraverseWith(*this);			
		}
		
		void Visit(const RegExAltern& n) override {
			// add the last chars of the left child
			n.GetLeftExpr().TraverseWith(*this);
			// add the last chars of the right child
			n.GetRightExpr().TraverseWith(*this);
		}

		void Visit(const RegExConcat& n) override {
			// add the last chars of the right child
			n.GetRightExpr().TraverseWith(*this);
			if(n.GetRightExpr().IsNullable()) {
				// add the last chars of the left child
				n.GetLeftExpr().TraverseWith(*this);
			}
		}
		/** @} */

	private:
		RegExCharList& chars;
	};
}

namespace parsec {
	RegExCharList RegExNode::GetFirstChars() const {
		RegExCharList chars;
		ComputeFirstChars firstChars(chars);
		TraverseWith(firstChars);
		return chars;
	}

	RegExCharList RegExNode::GetLastChars() const {
		RegExCharList chars;
		ComputeLastChars lastChars(chars);
		TraverseWith(lastChars);
		return chars;
	}

	void RegExChar::TraverseWith(RegExTraverser& traverser) const {
		TraverseNode(*this, traverser);
	}

	void RegExStar::TraverseWith(RegExTraverser& traverser) const {
		TraverseNode(*this, traverser);
	}
	
	void RegExAltern::TraverseWith(RegExTraverser& traverser) const {
		TraverseNode(*this, traverser);
	}

	void RegExConcat::TraverseWith(RegExTraverser& traverser) const {
		TraverseNode(*this, traverser);
	}

	std::ostream& operator<<(std::ostream& out, const RegExNode& regex) {
		RegExPrinter printer(out);
		regex.TraverseWith(printer);
		return out;
	}
}