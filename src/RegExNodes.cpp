#include "RegExNodes.hpp"
#include "RegExPrinter.hpp"

#include <sstream>

namespace {
	using namespace parsec;

	class ComputeFirstPos : public RegExTraverser {
	public:
		/** @{ */
		ComputeFirstPos(RegExCharList& chars) noexcept
		 : m_chars(chars)
		{ }
		/** @} */

	private:
		/** @{ */
		void VisitNode(const RegExChar& n) override {
			m_chars.push_back(&n);
		}

		void VisitNode(const RegExNil& n) override {
			// nothing to do
		}
		
		void VisitNode(const RegExOpt& n) override {
			// add the firstpos of the inner expression
			n.GetInnerExpr()->TraverseWith(*this);
		}

		void VisitNode(const RegExPlus& n) override {
			// add the firstpos of the inner expression
			n.GetInnerExpr()->TraverseWith(*this);			
		}

		void VisitNode(const RegExStar& n) override {
			// add the firstpos of the inner expression
			n.GetInnerExpr()->TraverseWith(*this);
		}
		
		void VisitNode(const RegExAltern& n) override {
			// add the firstpos of the left child
			n.GetLeftExpr()->TraverseWith(*this);
			// add the firstpos of the right child
			n.GetRightExpr()->TraverseWith(*this);
		}

		void VisitNode(const RegExConcat& n) override {
			// add the firstpos of the left child
			n.GetLeftExpr()->TraverseWith(*this);
			if(n.GetLeftExpr()->IsNullable()) {
				// add the firstpos of the right child
				n.GetRightExpr()->TraverseWith(*this);
			}
		}
		/** @} */

		RegExCharList& m_chars;
	};

	class ComputeLastPos : public RegExTraverser {
	public:
		/** @{ */
		ComputeLastPos(RegExCharList& chars) noexcept
		 : m_chars(chars)
		{ }
		/** @} */

	private:
		/** @{ */
		void VisitNode(const RegExChar& n) override {
			m_chars.push_back(&n);
		}
		
		void VisitNode(const RegExNil& n) override {
			// nothing to do
		}

		void VisitNode(const RegExOpt& n) override {
			// add the lastpos of the inner expression
			n.GetInnerExpr()->TraverseWith(*this);
		}
		
		void VisitNode(const RegExPlus& n) override {
			// add the lastpos of the inner expression
			n.GetInnerExpr()->TraverseWith(*this);
		}

		void VisitNode(const RegExStar& n) override {
			// add the lastpos of the inner expression
			n.GetInnerExpr()->TraverseWith(*this);
		}
		
		void VisitNode(const RegExAltern& n) override {
			// add the lastpos of the left child
			n.GetLeftExpr()->TraverseWith(*this);
			// add the lastpos of the right child
			n.GetRightExpr()->TraverseWith(*this);
		}

		void VisitNode(const RegExConcat& n) override {
			// add the lastpos of the right child
			n.GetRightExpr()->TraverseWith(*this);
			if(n.GetRightExpr()->IsNullable()) {
				// add the lastpos of the left child
				n.GetLeftExpr()->TraverseWith(*this);
			}
		}
		/** @} */

		RegExCharList& m_chars;
	};

	class ComputeFollowPos : public RegExTraverser {
	public:
		/** @{ */
		ComputeFollowPos(RegExCharList& chars) noexcept
		 : m_chars(chars)
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
			if(m_child == n.GetLeftExpr()) {
				AppendFirstPos(*n.GetRightExpr());
				if(n.GetRightExpr()->IsNullable()) {
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
				const auto oldChild = std::exchange(m_child, &n);
				parent->TraverseWith(*this);
				m_child = oldChild;
			}
		}
		
		void AppendFirstPos(const RegExNode& n) {
			const auto firstPos = n.GetFirstPos();
			m_chars.insert(m_chars.end(),
				firstPos.cbegin(), firstPos.cend()
			);
		}
		/** @} */

		const RegExNode* m_child = nullptr;
		RegExCharList& m_chars;
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

	std::string RegExNode::ToStr() const {
		return (std::ostringstream() << *this).str();
	}


	void RegExChar::TraverseWith(RegExTraverser& traverser) const {
		traverser.VisitNode(*this);
	}

	RegExCharList RegExChar::GetFollowPos() const {
		RegExCharList chars;
		ComputeFollowPos followPos(chars);
		TraverseWith(followPos);
		return chars;
	}


	void RegExNil::TraverseWith(RegExTraverser& traverser) const {
		traverser.VisitNode(*this);
	}

	void RegExOpt::TraverseWith(RegExTraverser& traverser) const {
		traverser.VisitNode(*this);
	}

	void RegExPlus::TraverseWith(RegExTraverser& traverser) const {
		traverser.VisitNode(*this);
	}

	void RegExStar::TraverseWith(RegExTraverser& traverser) const {
		traverser.VisitNode(*this);
	}
	
	void RegExAltern::TraverseWith(RegExTraverser& traverser) const {
		traverser.VisitNode(*this);
	}

	void RegExConcat::TraverseWith(RegExTraverser& traverser) const {
		traverser.VisitNode(*this);
	}


	std::ostream& operator<<(std::ostream& out, const RegExNode& n) {
		RegExPrinter(&out).Print(n);
		return out;
	}
}