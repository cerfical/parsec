#include "core/RegularExpr.hpp"

#include "regex/Parser.hpp"
#include "regex/nodes.hpp"

#include <algorithm>
#include <vector>

namespace parsec {
	using namespace regex;


	using AtomList = std::vector<const SymbolAtom*>;

	using IndexList = std::vector<int>;


	namespace {
		void computeFirstOrLastPos(const ExprNode& n, IndexList& indices, int nextAtomIndex, bool firstOrLast) {
			class Impl : NodeVisitor {
			public:

				Impl(IndexList& indices, int nextAtomIndex, bool firstOrLast)
					: m_indices(indices), m_nextAtomIndex(nextAtomIndex), m_firstOrLast(firstOrLast) {}

				void operator()(const ExprNode& n) {
					n.accept(*this);
				}

			private:
				void visit(const SymbolAtom& n) override {
					if(n.value()) {
						// only non-empty atoms must have positional indices assigned
						m_indices.push_back(m_nextAtomIndex);
					}
				}

				void visit(const PlusClosure& n) override {
					n.inner()->accept(*this);
				}

				void visit(const StarClosure& n) override {
					n.inner()->accept(*this);
				}

				void visit(const OptionalExpr& n) override {
					n.inner()->accept(*this);
				}

				void visit(const AlternExpr& n) override {
					computeFirstOrLastPos(*n.left(), m_indices, m_nextAtomIndex, m_firstOrLast);
					computeFirstOrLastPos(*n.right(), m_indices, m_nextAtomIndex + n.left()->atomCount(), m_firstOrLast);
				}

				void visit(const ConcatExpr& n) override {
					if(m_firstOrLast || n.right()->isNullable()) {
						computeFirstOrLastPos(*n.left(), m_indices, m_nextAtomIndex, m_firstOrLast);
					}

					if(!m_firstOrLast || n.left()->isNullable()) {
						computeFirstOrLastPos(*n.right(), m_indices, m_nextAtomIndex + n.left()->atomCount(), m_firstOrLast);
					}
				}


				IndexList& m_indices;

				int m_nextAtomIndex = {};
				bool m_firstOrLast = {};
			};
			
			Impl(indices, nextAtomIndex, firstOrLast)(n);
		}


		IndexList computeFirstPos(const ExprNode& n, int baseAtomIndex) {
			IndexList firstPos;
			computeFirstOrLastPos(n, firstPos, baseAtomIndex, true);
			return firstPos;
		}


		IndexList computeLastPos(const ExprNode& n, int baseAtomIndex) {
			IndexList lastPos;
			computeFirstOrLastPos(n, lastPos, baseAtomIndex, false);
			return lastPos;
		}


		std::vector<IndexList> computeFollowPos(const ExprNode& n) {
			class Impl : NodeVisitor {
			public:

				std::vector<IndexList> operator()(const ExprNode& n) {
					n.accept(*this);
					return std::move(m_followPos);
				}

			private:
				void visit(const SymbolAtom& n) override {
					m_atomCount++;
				}

				void visit(const PlusClosure& n) override {
					addFirstPosToLastPos(*n.inner(), *n.inner(), m_atomCount);
					n.inner()->accept(*this);
				}

				void visit(const StarClosure& n) override {
					addFirstPosToLastPos(*n.inner(), *n.inner(), m_atomCount);
					n.inner()->accept(*this);
				}

				void visit(const OptionalExpr& n) override {
					n.inner()->accept(*this);
				}

				void visit(const AlternExpr& n) override {
					n.left()->accept(*this);
					n.right()->accept(*this);
				}

				void visit(const ConcatExpr& n) override {
					const auto preLeftAtomCount = m_atomCount;
					n.left()->accept(*this);
					
					addFirstPosToLastPos(*n.left(), *n.right(), preLeftAtomCount);
					n.right()->accept(*this);
				}

				void addFirstPosToLastPos(const ExprNode& last, const ExprNode& first, int lastPosBaseAtomIndex) {
					const auto lastPos = computeLastPos(last, lastPosBaseAtomIndex);
					if(!lastPos.empty()) {
						const auto firstPos = computeFirstPos(first, m_atomCount);
						for(const auto& pos : lastPos) {
							if(pos >= m_followPos.size()) {
								m_followPos.resize(pos + 1);
							}
							m_followPos[pos].insert(m_followPos[pos].end(), firstPos.begin(), firstPos.end());
						}
					}
				}


				std::vector<IndexList> m_followPos;
				int m_atomCount = 0;
			} impl;
			return impl(n);
		}


		AtomList collectNonEmptyAtoms(const ExprNode& n) {
			class Impl : NodeVisitor {
			public:

				AtomList operator()(const ExprNode& n) {
					n.accept(*this);
					return std::move(m_atoms);
				}

			private:
				void visit(const SymbolAtom& n) override {
					if(n.value()) {
						m_atoms.push_back(&n);
					}
				}

				void visit(const PlusClosure& n) override {
					n.inner()->accept(*this);
				}

				void visit(const StarClosure& n) override {
					n.inner()->accept(*this);
				}

				void visit(const OptionalExpr& n) override {
					n.inner()->accept(*this);
				}

				void visit(const AlternExpr& n) override {
					n.left()->accept(*this);
					n.right()->accept(*this);
				}

				void visit(const ConcatExpr& n) override {
					n.left()->accept(*this);
					n.right()->accept(*this);
				}


				AtomList m_atoms;
			} impl;
			return impl(n);
		}
	}


	struct RegularExpr::ComputeCache {
		ComputeCache(NodePtr regex)
			: regexWithEndMark(regex, atom('$')) {}

		ConcatExpr regexWithEndMark;

		IndexList firstPos;
		std::vector<IndexList> followPos;
		AtomList atoms;
	};


	std::span<const int> RegularExpr::firstPos() const {
		if(const auto cachePtr = computeCache()) {
			if(cachePtr->firstPos.empty()) {
				cachePtr->firstPos = computeFirstPos(cachePtr->regexWithEndMark, 0);
			}
			return cachePtr->firstPos;
		}
		return {};
	}


	std::span<const int> RegularExpr::followPos(int pos) const {
		if(const auto cachePtr = computeCache()) {
			if(cachePtr->followPos.empty()) {
				cachePtr->followPos = computeFollowPos(cachePtr->regexWithEndMark);

				// make sure that the followpos sets are all sorted and contain only unique elements
				for(auto& fp : cachePtr->followPos) {
					const auto unique = (std::ranges::sort(fp), std::ranges::unique(fp));
					fp.erase(unique.begin(), unique.end());
				}
			}

			if(pos >= 0 && pos < cachePtr->followPos.size()) {
				return cachePtr->followPos[pos];
			}
		}
		return {};
	}


	const Symbol& RegularExpr::posValue(int pos) const {
		if(const auto cachePtr = computeCache()) {
			if(cachePtr->atoms.empty()) {
				cachePtr->atoms = collectNonEmptyAtoms(*m_regex);
			}
			if(pos >= 0 && pos < cachePtr->atoms.size()) {
				return cachePtr->atoms[pos]->value();
			}
		}

		static Symbol emptySymbol;
		return emptySymbol;
	}


	RegularExpr::ComputeCache* RegularExpr::computeCache() const {
		if(m_regex && !m_computeCache) {
			m_computeCache = std::make_shared<ComputeCache>(m_regex);
		}
		return m_computeCache.get();
	}


	RegularExpr::RegularExpr(std::string_view regex)
		: RegularExpr(RegularExpr(Parser().parse(regex))) {}
}