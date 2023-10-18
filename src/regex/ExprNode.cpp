#include "regex/ExprNode.hpp"

#include "regex/NodeTraverser.hpp"
#include "regex/NodePrinter.hpp"
#include "regex/nodes.hpp"


using namespace parsec::regex;


namespace {
	class ComputeFirstPos : public NodeTraverser {
	public:
		/** @{ */
		explicit ComputeFirstPos(CharLiteralList& chars) noexcept
			: m_chars(chars)
		{ }
		/** @} */


	private:
		/** @{ */
		void visitNode(const CharLiteral& n) override {
			m_chars.push_back(&n);
		}

		void visitNode(const NilExpr&) override {
			// nothing to do
		}

		void visitNode(const OptionalExpr& n) override {
			// add the firstpos of the inner expression
			n.inner()->traverseWith(*this);
		}

		void visitNode(const PlusExpr& n) override {
			// add the firstpos of the inner expression
			n.inner()->traverseWith(*this);
		}

		void visitNode(const StarExpr& n) override {
			// add the firstpos of the inner expression
			n.inner()->traverseWith(*this);
		}

		void visitNode(const AlternExpr& n) override {
			// add the firstpos of the left child
			n.left()->traverseWith(*this);
			// add the firstpos of the right child
			n.right()->traverseWith(*this);
		}

		void visitNode(const ConcatExpr& n) override {
			// add the firstpos of the left child
			n.left()->traverseWith(*this);
			if(n.left()->nullable()) {
				// add the firstpos of the right child
				n.right()->traverseWith(*this);
			}
		}
		/** @} */


		/** @{ */
		CharLiteralList& m_chars;
		/** @} */
	};

	class ComputeLastPos : public NodeTraverser {
	public:
		/** @{ */
		explicit ComputeLastPos(CharLiteralList& chars) noexcept
			: m_chars(chars)
		{ }
		/** @} */


	private:
		/** @{ */
		void visitNode(const CharLiteral& n) override {
			m_chars.push_back(&n);
		}

		void visitNode(const NilExpr&) override {
			// nothing to do
		}

		void visitNode(const OptionalExpr& n) override {
			// add the lastpos of the inner expression
			n.inner()->traverseWith(*this);
		}

		void visitNode(const PlusExpr& n) override {
			// add the lastpos of the inner expression
			n.inner()->traverseWith(*this);
		}

		void visitNode(const StarExpr& n) override {
			// add the lastpos of the inner expression
			n.inner()->traverseWith(*this);
		}

		void visitNode(const AlternExpr& n) override {
			// add the lastpos of the left child
			n.left()->traverseWith(*this);
			// add the lastpos of the right child
			n.right()->traverseWith(*this);
		}

		void visitNode(const ConcatExpr& n) override {
			// add the lastpos of the right child
			n.right()->traverseWith(*this);
			if(n.right()->nullable()) {
				// add the lastpos of the left child
				n.left()->traverseWith(*this);
			}
		}
		/** @} */


		/** @{ */
		CharLiteralList& m_chars;
		/** @} */
	};
}


namespace parsec::regex {
	std::ostream& operator<<(std::ostream& out, const ExprNode& n) {
		NodePrinter(out).print(n);
		return out;
	}


	CharLiteralList ExprNode::firstPos() const {
		CharLiteralList chars;
		ComputeFirstPos firstPos(chars);
		traverseWith(firstPos);
		return chars;
	}

	CharLiteralList ExprNode::lastPos() const {
		CharLiteralList chars;
		ComputeLastPos lastPos(chars);
		traverseWith(lastPos);
		return chars;
	}
}