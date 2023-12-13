#include "regex/ExprNode.hpp"

#include "regex/NodeVisitor.hpp"
#include "regex/nodes.hpp"

#include "utils/Chars.hpp"

namespace parsec::regex {
	namespace {
		class ExprPrinter : NodeVisitor {
		public:

			explicit ExprPrinter(std::ostream& out) noexcept
				: m_out(out)
			{ }

			void operator()(const ExprNode& n) {
				n.acceptVisitor(*this);
			}

		private:

			void visit(const CharAtom& n) override {
				m_out << utils::Chars::escape(n.value());
			}

			void visit(const NilExpr&) override {
				m_out << "()";
			}


			void visit(const OptionalExpr& n) override {
				n.inner()->acceptVisitor(*this);
				m_out << '?';
			}

			void visit(const PlusExpr& n) override {
				n.inner()->acceptVisitor(*this);
				m_out << '+';
			}

			void visit(const StarExpr& n) override {
				n.inner()->acceptVisitor(*this);
				m_out << '*';
			}


			void visit(const AlternExpr& n) override {
				m_out << '(';
				n.left()->acceptVisitor(*this);
				m_out << '|';
				n.right()->acceptVisitor(*this);
				m_out << ')';
			}

			void visit(const ConcatExpr& n) override {
				m_out << '(';
				n.left()->acceptVisitor(*this);
				n.right()->acceptVisitor(*this);
				m_out << ')';
			}


			std::ostream& m_out;
		};
	}

	std::ostream& operator<<(std::ostream& out, const ExprNode& n) {
		return (ExprPrinter(out)(n), out);
	}

}