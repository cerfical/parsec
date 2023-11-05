#include "fg/rules.hpp"

namespace parsec::fg {
	namespace {
		class ComputeLeadingAtoms : RuleTraverser {
		public:
			AtomList operator()(const Rule& n) {
				n.traverse(*this);
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
				n.inner()->traverse(*this);
			}

			void visit(const PlusRule& n) override {
				// add leading atoms of the inner expression
				n.inner()->traverse(*this);
			}

			void visit(const StarRule& n) override {
				// add leading atoms of the inner expression
				n.inner()->traverse(*this);
			}


			void visit(const RuleAltern& n) override {
				// add leading atoms of the left child
				n.left()->traverse(*this);
				// add leading atoms of the right child
				n.right()->traverse(*this);
			}

			void visit(const RuleConcat& n) override {
				// add leading atoms of the left child
				n.left()->traverse(*this);
				if(n.left()->nullable()) {
					// add leading atoms of the right child
					n.right()->traverse(*this);
				}
			}


			AtomList m_atoms;
		};

		class ComputeTrailingAtoms : RuleTraverser {
		public:
			AtomList operator()(const Rule& n) {
				n.traverse(*this);
				return std::move(m_atoms);
			}

		private:
			void visit(const Atom& n) override {
				m_atoms.push_back(&n);
			}

			void visit(const NilRule&) override {
				// nothing to do
			}


			void visit(const OptionalRule& n) override {
				// add trailing atoms of the inner expression
				n.inner()->traverse(*this);
			}

			void visit(const PlusRule& n) override {
				// add trailing atoms of the inner expression
				n.inner()->traverse(*this);
			}

			void visit(const StarRule& n) override {
				// add trailing atoms of the inner expression
				n.inner()->traverse(*this);
			}


			void visit(const RuleAltern& n) override {
				// add trailing atoms of the left child
				n.left()->traverse(*this);
				// add trailing atoms of the right child
				n.right()->traverse(*this);
			}

			void visit(const RuleConcat& n) override {
				// add trailing atoms of the right child
				n.right()->traverse(*this);
				if(n.right()->nullable()) {
					// add trailing atoms of the left child
					n.left()->traverse(*this);
				}
			}


			AtomList m_atoms;
		};
	
		class ComputeEndAtom : RuleTraverser {
		public:
			const Atom* operator()(const Rule& n) noexcept {
				n.traverse(*this);
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
				n.inner()->traverse(*this);
			}

			void visit(const PlusRule& n) override {
				n.inner()->traverse(*this);
			}

			void visit(const StarRule& n) override {
				n.inner()->traverse(*this);
			}


			void visit(const RuleAltern& n) override {
				n.right()->traverse(*this);
				if(!m_atom) {
					n.left()->traverse(*this);
				}
			}

			void visit(const RuleConcat& n) override {
				n.right()->traverse(*this);
				if(!m_atom) {
					n.left()->traverse(*this);
				}
			}


			const Atom* m_atom = nullptr;
		};

		class ComputeNullable : RuleTraverser {
		public:
			bool operator()(const Rule& n) noexcept {
				n.traverse(*this);
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
				n.left()->traverse(*this); // check if the left child is nullable
				if(!m_nullable) { // if it is not, check the right child
					n.right()->traverse(*this);
				}
			}

			void visit(const RuleConcat& n) override {
				n.left()->traverse(*this); // check if the left child is nullable
				if(m_nullable) { // if it is, check the right child
					n.right()->traverse(*this);
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

	AtomList Rule::trailingAtoms() const {
		return ComputeTrailingAtoms()(*this);
	}

	const Atom* Rule::endAtom() const noexcept {
		return ComputeEndAtom()(*this);
	}

	bool Rule::nullable() const noexcept {
		return ComputeNullable()(*this);
	}
}