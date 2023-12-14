#include "regex/Pattern.hpp"

#include "regex/Parser.hpp"
#include "regex/ExprTraverser.hpp"
#include "regex/nodes.hpp"

namespace parsec::regex {
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


		class FindEndAtom : ExprTraverser {
		public:

			const CharAtom* operator()(const ExprNode& n) noexcept {
				traverse(n);
				return m_endAtom;
			}

		private:

			void visit(const CharAtom& n) override {
				m_endAtom = &n;
			}

			void visit(const AlternExpr& n) override {
				visitBinary(n);
			}

			void visit(const ConcatExpr& n) override {
				visitBinary(n);
			}


			void visitBinary(const BinaryExpr& n) {
				// first try to find the end atom in the right child
				if(!(m_endAtom = FindEndAtom()(*n.right()))) {
					// and if there is none, repeat the search in the left child
					m_endAtom = FindEndAtom()(*n.left());
				}
			}


			const CharAtom* m_endAtom = {};
		};


		class FindRoots : ExprTraverser {
		public:

			Pattern::AtomList operator()(const ExprNode& n) {
				traverse(n);
				return std::move(m_roots);
			}

		private:

			void visit(const CharAtom& n) override {
				m_roots.push_back(&n);
			}

			void visit(const ConcatExpr& n) override {
				// add roots of the left child
				traverse(*n.left());
				if(IsNullable()(*n.left())) {
					// add roots of the right child
					traverse(*n.right());
				}
			}


			Pattern::AtomList m_roots;
		};


		class FindAllAtoms : ExprTraverser {
		public:

			Pattern::AtomList operator()(const ExprNode& n) {
				traverse(n);
				return std::move(m_atoms);
			}

		private:
			void visit(const CharAtom& n) override {
				m_atoms.push_back(&n);
			}

			Pattern::AtomList m_atoms;
		};


		class FindFollowers : NodeVisitor {
		public:

			Pattern::AtomList operator()(const CharAtom& n) {
				n.acceptVisitor(*this);
				return std::move(m_follows);
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
				appendRootsOf(n);
				traverseParent(n);
			}

			void visit(const StarExpr& n) override {
				appendRootsOf(n);
				traverseParent(n);
			}


			void visit(const AlternExpr& n) override {
				traverseParent(n);
			}

			void visit(const ConcatExpr& n) override {
				if(m_child == n.left()) {
					appendRootsOf(*n.right());
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

			void appendRootsOf(const ExprNode& n) {
				const auto follows = FindRoots()(n);
				m_follows.insert(m_follows.end(),
					follows.cbegin(),
					follows.cend()
				);
			}


			Pattern::AtomList m_follows;
			const ExprNode* m_child = {};
		};
	}


	ExprPtr Pattern::parseRegex(std::string_view regex) {
		return Parser().parse(regex);
	}


	const CharAtom* Pattern::endAtom() const noexcept {
		if(m_regex) {
			return FindEndAtom()(*m_regex);
		}
		return nullptr;
	}

	Pattern::AtomList Pattern::followersOf(const CharAtom* ch) const {
		return FindFollowers()(*ch);
	}

	Pattern::AtomList Pattern::rootAtoms() const {
		if(m_regex) {
			return FindRoots()(*m_regex);
		}
		return {};
	}

	Pattern::AtomList Pattern::atoms() const {
		if(m_regex) {
			return FindAllAtoms()(*m_regex);
		}
		return {};
	}

}