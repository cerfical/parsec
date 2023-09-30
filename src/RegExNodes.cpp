#include "RegExNodes.hpp"
#include "RegExPrinter.hpp"

namespace {
	using namespace parsec;


	template <typename Node>
	concept IsUnaryExpr = requires(const Node& n) { n.GetChild(); };

	template <typename Node>
	concept IsBinaryExpr = requires(const Node& n) { n.GetLeftChild(); n.GetRightChild(); };

	template <typename Node>
	void TraverseNode(const Node& n, RegExTraverser& traverser) {
		switch(traverser.GetTraversalType()) {
			case TraversalTypes::Preorder: {
				traverser.VisitNode(n);
				if constexpr(IsBinaryExpr<Node>) {
					n.GetLeftChild().TraverseWith(traverser);
					n.GetRightChild().TraverseWith(traverser);
				} else if constexpr(IsUnaryExpr<Node>) {
					n.GetChild().TraverseWith(traverser);
				} 
				break;
			}
			case TraversalTypes::Postorder: {
				if constexpr(IsBinaryExpr<Node>) {
					n.GetLeftChild().TraverseWith(traverser);
					n.GetRightChild().TraverseWith(traverser);
				} else if constexpr(IsUnaryExpr<Node>) {
					n.GetChild().TraverseWith(traverser);
				} 
				traverser.VisitNode(n);
				break;
			}
			case TraversalTypes::None: {
				traverser.VisitNode(n);
				break;
			}
		}
	}


	class ComputeFirstPos : public RegExTraverser {
	public:
		/** @{ */
		ComputeFirstPos(RegExCharList& chars) noexcept
		 : chars(chars)
		{ }
		/** @} */

	private:
		/** @{ */
		void VisitNode(const RegExChar& n) override {
			chars.push_back(&n);
		}

		void VisitNode(const RegExNil& n) override {
			// nothing to do
		}
		
		void VisitNode(const RegExOpt& n) override {
			// add the firstpos of the inner expression
			n.GetChild().TraverseWith(*this);			
		}

		void VisitNode(const RegExPlus& n) override {
			// add the firstpos of the inner expression
			n.GetChild().TraverseWith(*this);			
		}

		void VisitNode(const RegExStar& n) override {
			// add the firstpos of the inner expression
			n.GetChild().TraverseWith(*this);			
		}
		
		void VisitNode(const RegExAltern& n) override {
			// add the firstpos of the left child
			n.GetLeftChild().TraverseWith(*this);
			// add the firstpos of the right child
			n.GetRightChild().TraverseWith(*this);
		}

		void VisitNode(const RegExConcat& n) override {
			// add the firstpos of the left child
			n.GetLeftChild().TraverseWith(*this);
			if(n.GetLeftChild().IsNullable()) {
				// add the firstpos of the right child
				n.GetRightChild().TraverseWith(*this);
			}
		}
		/** @} */

		RegExCharList& chars;
	};


	class ComputeLastPos : public RegExTraverser {
	public:
		/** @{ */
		ComputeLastPos(RegExCharList& chars) noexcept
		 : chars(chars)
		{ }
		/** @} */

	private:
		/** @{ */
		void VisitNode(const RegExChar& n) override {
			chars.push_back(&n);
		}
		
		void VisitNode(const RegExNil& n) override {
			// nothing to do
		}

		void VisitNode(const RegExOpt& n) override {
			// add the lastpos of the inner expression
			n.GetChild().TraverseWith(*this);			
		}
		
		void VisitNode(const RegExPlus& n) override {
			// add the lastpos of the inner expression
			n.GetChild().TraverseWith(*this);			
		}

		void VisitNode(const RegExStar& n) override {
			// add the lastpos of the inner expression
			n.GetChild().TraverseWith(*this);			
		}
		
		void VisitNode(const RegExAltern& n) override {
			// add the lastpos of the left child
			n.GetLeftChild().TraverseWith(*this);
			// add the lastpos of the right child
			n.GetRightChild().TraverseWith(*this);
		}

		void VisitNode(const RegExConcat& n) override {
			// add the lastpos of the right child
			n.GetRightChild().TraverseWith(*this);
			if(n.GetRightChild().IsNullable()) {
				// add the lastpos of the left child
				n.GetLeftChild().TraverseWith(*this);
			}
		}
		/** @} */

		RegExCharList& chars;
	};


	class ComputeFollowPos : public RegExTraverser {
	public:
		/** @{ */
		ComputeFollowPos(RegExCharList& chars) noexcept
		 : chars(chars)
		{ }
		/** @} */

	private:
		/** @{ */
		void VisitNode(const RegExChar& n) override {
			TraverseParent(n);
		}

		void VisitNode(const RegExNil& n) override {
			// nothing to do
		}

		void VisitNode(const RegExOpt& n) override {
			TraverseParent(n);
		}

		void VisitNode(const RegExPlus& n) override {
			AppendFirstPos(n);
			TraverseParent(n);
		}

		void VisitNode(const RegExStar& n) override {
			AppendFirstPos(n);
			TraverseParent(n);
		}
		
		void VisitNode(const RegExAltern& n) override {
			TraverseParent(n);
		}

		void VisitNode(const RegExConcat& n) override {
			if(child == &n.GetLeftChild()) {
				AppendFirstPos(n.GetRightChild());
				if(n.GetRightChild().IsNullable()) {
					TraverseParent(n);
				}
			} else {
				TraverseParent(n);
			}
		}
		/** @} */

		/** @{ */
		void TraverseParent(const RegExNode& n) {
			if(const auto parent = n.GetParent()) {
				const auto oldChild = std::exchange(child, &n);
				parent->TraverseWith(*this);
				child = oldChild;
			}
		}
		
		void AppendFirstPos(const RegExNode& n) {
			const auto first = n.GetFirstPos();
			chars.insert(chars.end(),
				first.cbegin(), first.cend()
			);
		}
		/** @} */

		const RegExNode* child = nullptr;
		RegExCharList& chars;
	};
}

namespace parsec {
	RegExCharList RegExNode::GetFirstPos() const {
		RegExCharList chars;
		ComputeFirstPos firstPos(chars);
		TraverseWith(firstPos);
		return chars;
	}

	RegExCharList RegExNode::GetLastPos() const {
		RegExCharList chars;
		ComputeLastPos lastPos(chars);
		TraverseWith(lastPos);
		return chars;
	}


	void RegExChar::TraverseWith(RegExTraverser& traverser) const {
		TraverseNode(*this, traverser);
	}

	RegExCharList RegExChar::GetFollowPos() const {
		RegExCharList chars;
		ComputeFollowPos followPos(chars);
		TraverseWith(followPos);
		return chars;
	}


	void RegExNil::TraverseWith(RegExTraverser& traverser) const {
		TraverseNode(*this, traverser);
	}


	void RegExOpt::TraverseWith(RegExTraverser& traverser) const {
		TraverseNode(*this, traverser);
	}


	void RegExPlus::TraverseWith(RegExTraverser& traverser) const {
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


	std::ostream& operator<<(std::ostream& out, const RegExNode& n) {
		RegExPrinter printer(&out);
		n.TraverseWith(printer);
		return out;
	}
}