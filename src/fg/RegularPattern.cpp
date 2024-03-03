#include "fg/RegularPattern.hpp"
#include "regex/nodes.hpp"

using namespace parsec::regex::nodes;

namespace parsec::fg {
	namespace {
		class IsNullable : NodeVisitor {
		public:

			bool operator()(const ExprNode& n) noexcept {
				n.acceptVisitor(*this);
				return m_nullable;
			}

		private:
			void visit(const CharAtom&) override {
				m_nullable = false;
			}

			void visit(const NilExpr&) override {
				m_nullable = true;
			}


			void visit(const OptionalExpr&) override {
				m_nullable = true;
			}

			void visit(const PlusExpr&) override {
				m_nullable = false;
			}

			void visit(const StarExpr&) override {
				m_nullable = true;
			}


			void visit(const AlternExpr& n) override {
				m_nullable = IsNullable()(*n.left()) || IsNullable()(*n.right());
			}

			void visit(const ConcatExpr& n) override {
				m_nullable = IsNullable()(*n.left()) && IsNullable()(*n.right());
			}


			bool m_nullable = true;
		};


		class ComputeFirstPos : NodeVisitor {
		public:

			auto operator()(const ExprNode& n) {
				n.acceptVisitor(*this);
				return std::move(m_firstPos);
			}

		private:
			void visit(const CharAtom& n) override {
				m_firstPos.push_back(n.posIndex());
			}

			void visit(const NilExpr& n) {
				// nothing to do
			}


			void visit(const PlusExpr& n) {
				n.inner()->acceptVisitor(*this);
			}

			void visit(const StarExpr& n) {
				n.inner()->acceptVisitor(*this);
			}

			void visit(const OptionalExpr& n) {
				n.inner()->acceptVisitor(*this);
			}


			void visit(const AlternExpr& n) {
				n.left()->acceptVisitor(*this);
				n.right()->acceptVisitor(*this);
			}

			void visit(const ConcatExpr& n) override {
				// add firstpos of the left child
				n.left()->acceptVisitor(*this);
				if(IsNullable()(*n.left())) {
					// add firstpos of the right child
					n.right()->acceptVisitor(*this);
				}
			}


			std::vector<gsl::index> m_firstPos;
		};


		class ComputeLastPos : NodeVisitor {
		public:

			auto operator()(const ExprNode& n) {
				n.acceptVisitor(*this);
				return std::move(m_lastPos);
			}

		private:
			void visit(const CharAtom& n) override {
				m_lastPos.push_back(n.posIndex());
			}

			void visit(const NilExpr& n) {
				// nothing to do
			}


			void visit(const PlusExpr& n) {
				n.inner()->acceptVisitor(*this);
			}

			void visit(const StarExpr& n) {
				n.inner()->acceptVisitor(*this);
			}

			void visit(const OptionalExpr& n) {
				n.inner()->acceptVisitor(*this);
			}


			void visit(const AlternExpr& n) {
				n.left()->acceptVisitor(*this);
				n.right()->acceptVisitor(*this);
			}

			void visit(const ConcatExpr& n) override {
				// add lastpos of the right child
				n.right()->acceptVisitor(*this);
				if(IsNullable()(*n.right())) {
					// add lastpos of the left child
					n.left()->acceptVisitor(*this);
				}
			}


			std::vector<gsl::index> m_lastPos;
		};


		class ComputeFollowPos : NodeVisitor {
		public:

			auto operator()(const CharAtom& n) {
				n.acceptVisitor(*this);
				return std::move(m_followPos);
			}

		private:
			void visit(const CharAtom& n) override {
				traverseParent(n);
			}

			void visit(const NilExpr&) override {
				// nothing to do
			}


			void visit(const OptionalExpr& n) override {
				traverseParent(n);
			}

			void visit(const PlusExpr& n) override {
				appendFirstPos(n);
				traverseParent(n);
			}

			void visit(const StarExpr& n) override {
				appendFirstPos(n);
				traverseParent(n);
			}


			void visit(const AlternExpr& n) override {
				traverseParent(n);
			}

			void visit(const ConcatExpr& n) override {
				if(m_child == n.left()) {
					appendFirstPos(*n.right());
					if(IsNullable()(*n.right())) {
						traverseParent(n);
					}
				} else {
					traverseParent(n);
				}
			}


			void traverseParent(const ExprNode& n) {
				// recursively traverse the parent node to find all atoms following the given one
				if(n.parent()) {
					const auto oldChild = std::exchange(m_child, &n);
					n.parent()->acceptVisitor(*this);
					m_child = oldChild;
				}
			}

			void appendFirstPos(const ExprNode& n) {
				const auto firstPos = ComputeFirstPos()(n);
				m_followPos.insert(m_followPos.end(),
					firstPos.cbegin(),
					firstPos.cend()
				);
			}


			std::vector<gsl::index> m_followPos;
			const ExprNode* m_child = {};
		};
	

		class FindAllAtoms : NodeTraverser {
		public:

			auto operator()(const ExprNode& n) {
				traverse(n);
				return std::move(m_atoms);
			}

		private:
			void visit(const CharAtom& n) override {
				m_atoms.push_back(&n);
			}

			void visit(const NilExpr&) override {
				// nothing to do
			}


			std::vector<const CharAtom*> m_atoms;
		};
	}


	RegularPattern::RegularPattern(std::string name, regex::RegularExpr regex, int id)
		: m_regex(std::move(regex))
		, m_atoms(FindAllAtoms()(*m_regex.rootNode()))
		, m_name(std::move(name))
		, m_id(id)
	{ }


	std::vector<gsl::index> RegularPattern::followPos(gsl::index pos) const {
		if(pos < size()) {
			return ComputeFollowPos()(*m_atoms[pos]);
		}
		return {};
	}


	std::vector<gsl::index> RegularPattern::firstPos() const {
		return ComputeFirstPos()(*m_regex.rootNode());
	}


	std::vector<gsl::index> RegularPattern::lastPos() const {
		return ComputeLastPos()(*m_regex.rootNode());
	}
}