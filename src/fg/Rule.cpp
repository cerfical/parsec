#include "fg/rules.hpp"

#include "fg/RuleVisitor.hpp"
#include "fg/RulePrinter.hpp"

namespace parsec::fg {
	namespace {
		class ComputeLeadingAtoms : RuleVisitor {
		public:
			AtomList operator()(const Rule& n) {
				n.acceptVisitor(*this);
				return std::move(m_atoms);
			}

		private:
			void visit(const Atom& n) override {
				m_atoms.push_back(&n);
			}

			void visit(const NilRule&) override {
				// nothing to don
			}


			void visit(const OptionalRule& n) override {
				// add leading atoms of the inner expression
				n.inner()->acceptVisitor(*this);
			}

			void visit(const PlusRule& n) override {
				// add leading atoms of the inner expression
				n.inner()->acceptVisitor(*this);
			}

			void visit(const StarRule& n) override {
				// add leading atoms of the inner expression
				n.inner()->acceptVisitor(*this);
			}


			void visit(const RuleAltern& n) override {
				// add leading atoms of the left child
				n.left()->acceptVisitor(*this);
				// add leading atoms of the right child
				n.right()->acceptVisitor(*this);
			}

			void visit(const RuleConcat& n) override {
				// add leading atoms of the left child
				n.left()->acceptVisitor(*this);
				if(n.left()->isNullable()) {
					// add leading atoms of the right child
					n.right()->acceptVisitor(*this);
				}
			}


			AtomList m_atoms;
		};

		class FindEndAtom : RuleVisitor {
		public:
			const Atom* operator()(const Rule& n) noexcept {
				n.acceptVisitor(*this);
				return m_atom;
			}

		private:
			void visit(const Atom& n) override {
				m_atom = &n;
			}

			void visit(const NilRule&) override {
				// nothing to do
			}


			void visit(const OptionalRule& n) override {
				n.inner()->acceptVisitor(*this);
			}

			void visit(const PlusRule& n) override {
				n.inner()->acceptVisitor(*this);
			}

			void visit(const StarRule& n) override {
				n.inner()->acceptVisitor(*this);
			}


			void visit(const RuleAltern& n) override {
				n.right()->acceptVisitor(*this);
				if(!m_atom) {
					n.left()->acceptVisitor(*this);
				}
			}

			void visit(const RuleConcat& n) override {
				n.right()->acceptVisitor(*this);
				if(!m_atom) {
					n.left()->acceptVisitor(*this);
				}
			}


			const Atom* m_atom = nullptr;
		};

		class IsNullable : RuleVisitor {
		public:
			bool operator()(const Rule& n) noexcept {
				n.acceptVisitor(*this);
				return m_nullable;
			}

		private:
			void visit(const Atom&) override {
				m_nullable = false;
			}

			void visit(const NilRule&) override {
				m_nullable = true;
			}


			void visit(const OptionalRule&) override {
				m_nullable = true;
			}

			void visit(const PlusRule&) override {
				m_nullable = false;
			}

			void visit(const StarRule&) override {
				m_nullable = true;
			}


			void visit(const RuleAltern& n) override {
				n.left()->acceptVisitor(*this); // check if the left child is nullable
				if(!m_nullable) { // if it is not, check the right child
					n.right()->acceptVisitor(*this);
				}
			}

			void visit(const RuleConcat& n) override {
				n.left()->acceptVisitor(*this); // check if the left child is nullable
				if(m_nullable) { // if it is, check the right child
					n.right()->acceptVisitor(*this);
				}
			}


			bool m_nullable = false;
		};
	}


	std::ostream& operator<<(std::ostream& out, const Rule& n) {
		RulePrinter(out).print(n);
		return out;
	}


	AtomList Rule::leadingAtoms() const {
		return ComputeLeadingAtoms()(*this);
	}

	const Atom* Rule::endAtom() const noexcept {
		return FindEndAtom()(*this);
	}

	bool Rule::isNullable() const noexcept {
		return IsNullable()(*this);
	}
}