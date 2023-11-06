#include "fg/rules.hpp"

namespace parsec::fg {
	namespace {
		class ComputeNextAtoms : RuleTraverser {
		public:
			/** @{ */
			AtomList operator()(const Rule& n) {
				n.traverse(*this);
				return std::move(m_atoms);
			}
			/** @} */


		private:
			/** @{ */
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
			/** @} */


			/** @{ */
			void traverseParent(const Rule& n) {
				// recursively traverse the parent node to find all atoms following the given one
				if(n.parent()) {
					const auto oldChild = std::exchange(m_child, &n);
					n.parent()->traverse(*this);
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
			/** @} */


			/** @{ */
			AtomList m_atoms;
			const Rule* m_child = nullptr;
			/** @} */
		};
	}


	void Atom::traverse(RuleTraverser& traverser) const {
		traverser.visit(*this);
	}

	AtomList Atom::nextAtoms() const {
		return ComputeNextAtoms()(*this);
	}
}