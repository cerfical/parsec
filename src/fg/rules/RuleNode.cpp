#include "fg/rules/rules.hpp"

namespace parsec::fg::rules {
	namespace {
		class ComputeFirstPos : public RuleTraverser {
		public:
			/** @{ */
			RulePosList positions;
			/** @} */


		private:
			/** @{ */
			void visitNode(const CharLiteral& n) override {
				positions.push_back(&n);
			}

			void visitNode(const RuleRef& n) override {
				positions.push_back(&n);
			}

			void visitNode(const NilRule&) override {
				// nothing to do
			}

			void visitNode(const OptRule& n) override {
				// add the firstpos of the inner expression
				n.inner()->traverseWith(*this);
			}

			void visitNode(const PlusRule& n) override {
				// add the firstpos of the inner expression
				n.inner()->traverseWith(*this);
			}

			void visitNode(const StarRule& n) override {
				// add the firstpos of the inner expression
				n.inner()->traverseWith(*this);
			}

			void visitNode(const RuleAltern& n) override {
				// add the firstpos of the left child
				n.left()->traverseWith(*this);
				// add the firstpos of the right child
				n.right()->traverseWith(*this);
			}

			void visitNode(const RuleConcat& n) override {
				// add the firstpos of the left child
				n.left()->traverseWith(*this);
				if(n.left()->nullable()) {
					// add the firstpos of the right child
					n.right()->traverseWith(*this);
				}
			}
			/** @} */
		};

		class ComputeLastPos : public RuleTraverser {
		public:
			/** @{ */
			RulePosList positions;
			/** @} */


		private:
			/** @{ */
			void visitNode(const CharLiteral& n) override {
				positions.push_back(&n);
			}

			void visitNode(const RuleRef& n) override {
				positions.push_back(&n);
			}

			void visitNode(const NilRule&) override {
				// nothing to do
			}

			void visitNode(const OptRule& n) override {
				// add the lastpos of the inner expression
				n.inner()->traverseWith(*this);
			}

			void visitNode(const PlusRule& n) override {
				// add the lastpos of the inner expression
				n.inner()->traverseWith(*this);
			}

			void visitNode(const StarRule& n) override {
				// add the lastpos of the inner expression
				n.inner()->traverseWith(*this);
			}

			void visitNode(const RuleAltern& n) override {
				// add the lastpos of the left child
				n.left()->traverseWith(*this);
				// add the lastpos of the right child
				n.right()->traverseWith(*this);
			}

			void visitNode(const RuleConcat& n) override {
				// add the lastpos of the right child
				n.right()->traverseWith(*this);
				if(n.right()->nullable()) {
					// add the lastpos of the left child
					n.left()->traverseWith(*this);
				}
			}
			/** @} */
		};
	}


	std::ostream& operator<<(std::ostream& out, const RuleNode& n) {
		RulePrinter(out).print(n);
		return out;
	}


	RulePosList RuleNode::firstPos() const {
		ComputeFirstPos firstPos;
		traverseWith(firstPos);
		return firstPos.positions;
	}

	RulePosList RuleNode::lastPos() const {
		ComputeLastPos lastPos;
		traverseWith(lastPos);
		return lastPos.positions;
	}
}