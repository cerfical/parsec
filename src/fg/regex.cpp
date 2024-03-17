#include "fg/regex.hpp"
#include "utils/string_utils.hpp"

namespace parsec::fg::regex {
	namespace {
		class NodePrinter : private NodeVisitor {
		public:

			explicit NodePrinter(std::ostream& out) noexcept
				: m_out(out) {}

			void print(const ExprNode& n) {
				n.accept(*this);
			}

		private:
			void visit(const SymbolAtom& n) override {
				m_out << '\'' << string_utils::escape(n.symbol().name()) << '\'';
			}

			void visit(const OptionalExpr& n) override {
				n.inner()->accept(*this);
				m_out << '?';
			}

			void visit(const PlusClosure& n) override {
				n.inner()->accept(*this);
				m_out << '+';
			}

			void visit(const StarClosure& n) override {
				n.inner()->accept(*this);
				m_out << '*';
			}

			void visit(const AlternExpr& n) override {
				m_out << '(';
				n.left()->accept(*this);
				m_out << " | ";
				n.right()->accept(*this);
				m_out << ')';
			}

			void visit(const ConcatExpr& n) override {
				m_out << '(';
				n.left()->accept(*this);
				m_out << ' ';
				n.right()->accept(*this);
				m_out << ')';
			}

			std::ostream& m_out;
		};
	}


	void ExprNode::printTo(std::ostream& out) const {
		NodePrinter(out).print(*this);
	}


	void SymbolAtom::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void StarClosure::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void PlusClosure::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void OptionalExpr::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void ConcatExpr::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void AlternExpr::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}