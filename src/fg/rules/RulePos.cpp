#include "fg/rules/rules.hpp"

namespace parsec::fg::rules {
	namespace {
		class ComputeFollowPos : public RuleTraverser {
		public:
			/** @{ */
			RulePosList positions;
			/** @} */


		private:
			/** @{ */
			void visitNode(const CharLiteral& n) override {
				traverseParent(n);
			}

			void visitNode(const RuleRef& n) override {
				traverseParent(n);
			}

			void visitNode(const NilRule&) override {
				// nothing to do
			}

			void visitNode(const OptRule& n) override {
				traverseParent(n);
			}

			void visitNode(const PlusRule& n) override {
				appendFirstPos(n);
				traverseParent(n);
			}

			void visitNode(const StarRule& n) override {
				appendFirstPos(n);
				traverseParent(n);
			}

			void visitNode(const RuleAltern& n) override {
				traverseParent(n);
			}

			void visitNode(const RuleConcat& n) override {
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
			void traverseParent(const RuleNode& n) {
				if(n.parent()) {
					const auto oldChild = std::exchange(m_child, &n);
					n.parent()->traverseWith(*this);
					m_child = oldChild;
				}
			}

			void appendFirstPos(const RuleNode& n) {
				const auto firstPos = n.firstPos();
				positions.insert(positions.end(),
					firstPos.cbegin(), firstPos.cend()
				);
			}
			/** @} */


			/** @{ */
			const RuleNode* m_child = nullptr;
			/** @} */
		};
	}


	RulePosList RulePos::followPos() const {
		ComputeFollowPos followPos;
		traverseWith(followPos);
		return followPos.positions;
	}

	bool RulePos::nullable() const noexcept {
		return false;
	}
}