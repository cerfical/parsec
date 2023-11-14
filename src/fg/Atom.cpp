#include "fg/rules.hpp"

namespace parsec::fg {
	namespace {
		class ComputeNextAtoms : RuleVisitor {
		public:
			AtomList operator()(const Rule& n) {
				n.acceptVisitor(*this);
				return std::move(m_atoms);
			}

		private:
			void visit(const Atom& n) override {
				traverseParent(n);
			}

			void visit(const NilRule&) override {
				// nothing to do
			}


			void visit(const OptionalRule& n) override {
				traverseParent(n);
			}

			void visit(const PlusRule& n) override {
				storeLeadingAtoms(n);
				traverseParent(n);
			}

			void visit(const StarRule& n) override {
				storeLeadingAtoms(n);
				traverseParent(n);
			}


			void visit(const RuleAltern& n) override {
				traverseParent(n);
			}

			void visit(const RuleConcat& n) override {
				if(m_child == n.left()) {
					storeLeadingAtoms(*n.right());
					if(n.right()->isNullable()) {
						traverseParent(n);
					}
				} else {
					traverseParent(n);
				}
			}


			void traverseParent(const Rule& n) {
				// recursively traverse the parent node to find all atoms following the given one
				if(n.parent()) {
					const auto oldChild = std::exchange(m_child, &n);
					n.parent()->acceptVisitor(*this);
					m_child = oldChild;
				}
			}

			void storeLeadingAtoms(const Rule& n) {
				const auto atoms = n.leadingAtoms();
				m_atoms.insert(
					m_atoms.end(),
					atoms.cbegin(),
					atoms.cend()
				);
			}


			const Rule* m_child = nullptr;
			AtomList m_atoms;
		};
	}


	void Atom::acceptVisitor(RuleVisitor& visitor) const {
		visitor.visit(*this);
	}

	AtomList Atom::nextAtoms() const {
		return ComputeNextAtoms()(*this);
	}
}