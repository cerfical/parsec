#include "regex/ExprNode.hpp"

#include "regex/Traverser.hpp"
#include "regex/Printer.hpp"
#include "regex/nodes.hpp"

#include <sstream>

namespace {
	using namespace parsec::regex;
	
	class ComputeFirstPos : public Traverser {
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
			n.innerSubexpr()->traverseWith(*this);
		}

		void visitNode(const PlusExpr& n) override {
			// add the firstpos of the inner expression
			n.innerSubexpr()->traverseWith(*this);
		}

		void visitNode(const StarExpr& n) override {
			// add the firstpos of the inner expression
			n.innerSubexpr()->traverseWith(*this);
		}

		void visitNode(const AlternExpr& n) override {
			// add the firstpos of the left child
			n.leftSubexpr()->traverseWith(*this);
			// add the firstpos of the right child
			n.rightSubexpr()->traverseWith(*this);
		}

		void visitNode(const ConcatExpr& n) override {
			// add the firstpos of the left child
			n.leftSubexpr()->traverseWith(*this);
			if(n.leftSubexpr()->nullable()) {
				// add the firstpos of the right child
				n.rightSubexpr()->traverseWith(*this);
			}
		}
		/** @} */

		CharLiteralList& m_chars;
	};

	class ComputeLastPos : public Traverser {
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
			n.innerSubexpr()->traverseWith(*this);
		}

		void visitNode(const PlusExpr& n) override {
			// add the lastpos of the inner expression
			n.innerSubexpr()->traverseWith(*this);
		}

		void visitNode(const StarExpr& n) override {
			// add the lastpos of the inner expression
			n.innerSubexpr()->traverseWith(*this);
		}

		void visitNode(const AlternExpr& n) override {
			// add the lastpos of the left child
			n.leftSubexpr()->traverseWith(*this);
			// add the lastpos of the right child
			n.rightSubexpr()->traverseWith(*this);
		}

		void visitNode(const ConcatExpr& n) override {
			// add the lastpos of the right child
			n.rightSubexpr()->traverseWith(*this);
			if(n.rightSubexpr()->nullable()) {
				// add the lastpos of the left child
				n.leftSubexpr()->traverseWith(*this);
			}
		}
		/** @} */

		CharLiteralList& m_chars;
	};
}

namespace parsec::regex {
	std::ostream& operator<<(std::ostream& out, const ExprNode& n) {
		Printer(&out).print(n);
		return out;
	}


	CharLiteralList ExprNode::computeFirstPos() const {
		CharLiteralList chars;
		ComputeFirstPos firstPos(chars);
		traverseWith(firstPos);
		return chars;
	}

	CharLiteralList ExprNode::computeLastPos() const {
		CharLiteralList chars;
		ComputeLastPos lastPos(chars);
		traverseWith(lastPos);
		return chars;
	}

	std::string ExprNode::toStr() const {
		return (std::ostringstream() << *this).str();
	}
}