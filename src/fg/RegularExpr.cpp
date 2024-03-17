#include "fg/RegularExpr.hpp"
#include "fg/regex.hpp"

#include <algorithm>
#include <sstream>

namespace parsec::fg {
	using namespace regex;


	RegularExpr RegularExpr::fromPatternString(std::string_view str) {
		return Parser().parse(str);
	}


	RegularExpr RegularExpr::fromRawString(std::string_view str) {
		RegularExpr expr;
		if(!str.empty()) {
			expr = Symbol(str.front());
			for(std::size_t i = 1; i < str.size(); i++) {
				expr += Symbol(str[i]);
			}
		}
		return expr;
	}


	std::string RegularExpr::toString() const {
		std::ostringstream out;
		m_rootNode->printTo(out);
		return out.str();
	}


	RegularExpr::AtomList RegularExpr::firstAtoms() const {
		AtomList atoms;
		std::size_t posIndex = 0;
		
		computeFirstPos(m_rootNode, m_rootNode.get(), atoms, posIndex);
		
		return atoms;
	}


	RegularExpr::AtomList RegularExpr::Atom::followAtoms() const {
		class Impl : private NodeVisitor {
			enum class Status {
				Completed,
				AtomFoundInLastPos,
				AtomNotFound
			};

		public:
			Impl(const Atom& atom, AtomList& followPos) noexcept
				: m_atom(atom), m_followPos(followPos) {}

			void operator()() {
				if(m_atom.m_rootNode->accept(*this); m_status == Status::AtomFoundInLastPos) {
					// add an empty symbol to the set to represent the end marker
					m_followPos.push_back(Atom(m_posIndex));
				}
			}

		private:
			void visit(const SymbolAtom& n) override {
				if(n.symbol()) {
					if(m_atom.m_posIndex == m_posIndex) {
						m_status = Status::AtomFoundInLastPos;
					} else {
						m_status = Status::AtomNotFound;
					}
					// only non-empty symbols must have positional indices assigned to them
					m_posIndex++;
				} else {
					m_status = Status::AtomNotFound;
				}
			}

			void visit(const OptionalExpr& n) override {
				n.inner()->accept(*this);
			}

			void visit(const PlusClosure& n) override {
				auto posIndex = m_posIndex;
				if(n.inner()->accept(*this); m_status == Status::AtomFoundInLastPos) {
					computeFirstPos(m_atom.m_rootNode, n.inner(), m_followPos, posIndex);
				}
			}

			void visit(const StarClosure& n) override {
				auto posIndex = m_posIndex;
				if(n.inner()->accept(*this); m_status == Status::AtomFoundInLastPos) {
					computeFirstPos(m_atom.m_rootNode, n.inner(), m_followPos, posIndex);
				}
			}

			void visit(const AlternExpr& n) override {
				if(n.left()->accept(*this); m_status == Status::AtomNotFound) {
					n.right()->accept(*this);
				}
			}

			void visit(const ConcatExpr& n) override {
				if(n.left()->accept(*this); m_status == Status::AtomFoundInLastPos) {
					computeFirstPos(m_atom.m_rootNode, n.right(), m_followPos, m_posIndex);
					if(!n.right()->isNullable()) {
						m_status = Status::Completed;
					}
				} else if(m_status == Status::AtomNotFound) {
					n.right()->accept(*this);
				}
			}

			std::size_t m_posIndex = 0;
			Status m_status = {};

			AtomList& m_followPos;
			const Atom& m_atom;
		};

		AtomList atoms;
		if(m_atomNode) {
			Impl(*this, atoms)();

			// make sure that all atoms are unique and sorted in ascending order of their positions
			std::ranges::sort(atoms, [](const auto& lhs, const auto& rhs) {
				return lhs.posIndex() < rhs.posIndex();
			});
			const auto duplicates = std::ranges::unique(atoms, [](const auto& lhs, const auto& rhs) {
				return lhs.posIndex() == rhs.posIndex();
			});
			atoms.erase(duplicates.begin(), duplicates.end());
		}
		return atoms;
	}


	void RegularExpr::computeFirstPos(NodePtr rootNode, const regex::ExprNode* node, AtomList& firstPos, std::size_t& posIndex) {
		class Impl : private NodeVisitor {
		public:
			Impl(NodePtr rootNode, const regex::ExprNode* node, AtomList& firstPos, std::size_t& posIndex) noexcept
				: m_firstPos(firstPos), m_posIndex(posIndex), m_node(node), m_rootNode(rootNode) {}

			void operator()() {
				m_node->accept(*this);
				if(m_node->isNullable()) {
					// add an empty symbol to the set to represent the end marker
					m_firstPos.push_back(Atom(m_posIndex));
				}
			}

		private:
			void visit(const SymbolAtom& n) override {
				if(n.symbol()) {
					if(!m_ignoreSymbols) {
						m_firstPos.push_back(Atom(m_rootNode, &n, m_posIndex));
					}
					// only non-empty symbols must have positional indices assigned to them
					m_posIndex++;
				}
			}

			void visit(const OptionalExpr& n) override {
				n.inner()->accept(*this);
			}

			void visit(const PlusClosure& n) override {
				n.inner()->accept(*this);
			}

			void visit(const StarClosure& n) override {
				n.inner()->accept(*this);
			}

			void visit(const AlternExpr& n) override {
				n.left()->accept(*this);
				n.right()->accept(*this);
			}

			void visit(const ConcatExpr& n) override {
				n.left()->accept(*this);

				const auto ignoreSymbols = std::exchange(m_ignoreSymbols, m_ignoreSymbols || !n.left()->isNullable());
				n.right()->accept(*this);
				m_ignoreSymbols = ignoreSymbols;
			}

			AtomList& m_firstPos;
			std::size_t& m_posIndex;
			bool m_ignoreSymbols = false;

			const regex::ExprNode* m_node;
			NodePtr m_rootNode;
		};

		Impl(rootNode, node, firstPos, posIndex)();
	}


	const Symbol& RegularExpr::Atom::symbol() const {
		if(!m_atomNode) {
			static Symbol emptySymbol;
			return emptySymbol;
		}
		return m_atomNode->symbol();
	}


	RegularExpr altern(const RegularExpr& left, const RegularExpr& right) {
		return RegularExpr(makeNode<AlternExpr>(left.m_rootNode, right.m_rootNode));
	}


	RegularExpr concat(const RegularExpr& left, const RegularExpr& right) {
		return RegularExpr(makeNode<ConcatExpr>(left.m_rootNode, right.m_rootNode));
	}


	RegularExpr starClosure(const RegularExpr& expr) {
		return RegularExpr(makeNode<StarClosure>(expr.m_rootNode));
	}


	RegularExpr plusClosure(const RegularExpr& expr) {
		return RegularExpr(makeNode<PlusClosure>(expr.m_rootNode));
	}


	RegularExpr optional(const RegularExpr& expr) {
		return RegularExpr(makeNode<OptionalExpr>(expr.m_rootNode));
	}
}