#include "regex/nodes.hpp"
#include "regex/Traverser.hpp"

namespace {
	using namespace parsec::regex;

	class ComputeFollowPos : public Traverser {
	public:
		/** @{ */
		explicit ComputeFollowPos(CharLiteralList& chars) noexcept
			: m_chars(chars)
		{ }
		/** @} */

	private:
		/** @{ */
		void visitNode(const CharLiteral& n) override {
			traverseParent(n);
		}

		void visitNode(const NilExpr&) override {
			// nothing to do
		}

		void visitNode(const OptionalExpr& n) override {
			traverseParent(n);
		}

		void visitNode(const PlusExpr& n) override {
			appendFirstPos(n);
			traverseParent(n);
		}

		void visitNode(const StarExpr& n) override {
			appendFirstPos(n);
			traverseParent(n);
		}

		void visitNode(const AlternExpr& n) override {
			traverseParent(n);
		}

		void visitNode(const ConcatExpr& n) override {
			if(m_child == n.leftSubexpr()) {
				appendFirstPos(*n.rightSubexpr());
				if(n.rightSubexpr()->nullable()) {
					traverseParent(n);
				}
			} else {
				traverseParent(n);
			}
		}
		/** @} */

		/** @{ */
		void traverseParent(const ExprNode& n) {
			if(n.parent()) {
				const auto oldChild = std::exchange(m_child, &n);
				n.parent()->traverseWith(*this);
				m_child = oldChild;
			}
		}

		void appendFirstPos(const ExprNode& n) {
			const auto firstPos = n.computeFirstPos();
			m_chars.insert(m_chars.end(),
				firstPos.cbegin(), firstPos.cend()
			);
		}
		/** @} */

		const ExprNode* m_child = nullptr;
		CharLiteralList& m_chars;
	};
}

namespace parsec::regex {
	void CharLiteral::traverseWith(Traverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool CharLiteral::nullable() const noexcept {
		return false;
	}

	CharLiteralList CharLiteral::computeFollowPos() const {
		CharLiteralList chars;
		ComputeFollowPos followPos(chars);
		traverseWith(followPos);
		return chars;
	}


	void NilExpr::traverseWith(Traverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool NilExpr::nullable() const noexcept {
		return true;
	}


	void OptionalExpr::traverseWith(Traverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool OptionalExpr::nullable() const noexcept {
		return true;
	}


	void PlusExpr::traverseWith(Traverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool PlusExpr::nullable() const noexcept {
		return false;
	}


	void StarExpr::traverseWith(Traverser& traverser) const {
		traverser.visitNode(*this);
	}
	
	bool StarExpr::nullable() const noexcept {
		return true;
	}


	void AlternExpr::traverseWith(Traverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool AlternExpr::nullable() const noexcept {
		return leftSubexpr()->nullable() || rightSubexpr()->nullable();
	}


	void ConcatExpr::traverseWith(Traverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool ConcatExpr::nullable() const noexcept {
		return leftSubexpr()->nullable() && rightSubexpr()->nullable();
	}
}