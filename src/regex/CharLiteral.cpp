#include "regex/CharLiteral.hpp"
#include "regex/NodeTraverser.hpp"
#include "regex/nodes.hpp"


using namespace parsec::regex;


namespace {
	class ComputeFollowPos : public NodeTraverser {
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
			if(m_child == n.left()) {
				appendFirstPos(*n.right());
				if(n.right()->nullable()) {
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
			const auto firstPos = n.firstPos();
			m_chars.insert(m_chars.end(),
				firstPos.cbegin(), firstPos.cend()
			);
		}
		/** @} */


		/** @{ */
		const ExprNode* m_child = nullptr;
		CharLiteralList& m_chars;
		/** @} */
	};
}


namespace parsec::regex {
	CharLiteralList CharLiteral::followPos() const {
		CharLiteralList chars;
		ComputeFollowPos followPos(chars);
		traverseWith(followPos);
		return chars;
	}

	void CharLiteral::traverseWith(NodeTraverser& traverser) const {
		traverser.visitNode(*this);
	}

	bool CharLiteral::nullable() const noexcept {
		return false;
	}
}