#include "regex/nodes/ExprNode.hpp"

#include "regex/nodes/NodeVisitor.hpp"
#include "regex/nodes.hpp"

#include "utils/char_utils.hpp"

namespace parsec::regex::nodes {
	namespace {
		class PrintExpr : NodeVisitor {
		public:

			explicit PrintExpr(std::ostream& out) noexcept
				: m_out(out) {
			}

			void operator()(const ExprNode& n) {
				n.acceptVisitor(*this);
			}

		private:

			void visit(const CharAtom& n) override {
				m_out << char_utils::escape(n.value);
			}

			void visit(const NilExpr&) override {
				m_out << "()";
			}

			void visit(const OptionalExpr& n) override {
				n.inner->acceptVisitor(*this);
				m_out << '?';
			}

			void visit(const PlusExpr& n) override {
				n.inner->acceptVisitor(*this);
				m_out << '+';
			}

			void visit(const StarExpr& n) override {
				n.inner->acceptVisitor(*this);
				m_out << '*';
			}

			void visit(const AlternExpr& n) override {
				m_out << '(';
				n.left->acceptVisitor(*this);
				m_out << '|';
				n.right->acceptVisitor(*this);
				m_out << ')';
			}

			void visit(const ConcatExpr& n) override {
				m_out << '(';
				n.left->acceptVisitor(*this);
				n.right->acceptVisitor(*this);
				m_out << ')';
			}


			std::ostream& m_out;
		};
	

		class CompareEqual : NodeVisitor {
		public:

			bool operator()(const ExprNode* lhs, const ExprNode* rhs) noexcept {
				m_lhs = lhs;

				if(rhs) {
					m_equal = false;
					rhs->acceptVisitor(*this);
				} else if(lhs) {
					m_equal = false;
				} else {
					m_equal = true;
				}

				return m_equal;
			}

		private:

			void visit(const CharAtom& n) override {
				const auto lhs = dynamic_cast<const CharAtom*>(m_lhs);
				if(lhs && n.value == lhs->value) {
					m_equal = true;
				}
			}

			void visit(const NilExpr&) override {
				if(dynamic_cast<const NilExpr*>(m_lhs)) {
					m_equal = true;
				}
			}

			void visit(const OptionalExpr& n) override {
				visitUnary(n);
			}

			void visit(const PlusExpr& n) override {
				visitUnary(n);
			}

			void visit(const StarExpr& n) override {
				visitUnary(n);
			}

			void visit(const AlternExpr& n) override {
				visitBinary(n);
			}

			void visit(const ConcatExpr& n) override {
				visitBinary(n);
			}


			template <typename Node>
			void visitUnary(const Node& n) {
				const auto lhs = dynamic_cast<const Node*>(m_lhs);
				if(lhs) {
					m_lhs = lhs->inner.get();
					n.inner->acceptVisitor(*this);
				}
			}

			template <typename Node>
			void visitBinary(const Node& n) {
				const auto lhs = dynamic_cast<const Node*>(m_lhs);
				if(lhs) {
					m_lhs = lhs->left.get();
					n.left->acceptVisitor(*this);
					if(m_equal) {
						m_lhs = lhs->right.get();
						n.right->acceptVisitor(*this);
					}
				}
			}


			const ExprNode* m_lhs = {};
			bool m_equal = false;
		};
	}


	bool ExprNode::isEqualTo(const ExprNode& other) const noexcept {
		return CompareEqual()(this, &other);
	}


	void ExprNode::printTo(std::ostream& out) const {
		PrintExpr{ out }(*this);
	}

}