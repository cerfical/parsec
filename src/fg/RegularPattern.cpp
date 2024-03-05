#include "fg/RegularPattern.hpp"

#include "regex/nodes.hpp"
#include <algorithm>

using namespace parsec::regex::nodes;

namespace parsec::fg {
	class RegularPattern::IsNullable : NodeVisitor {
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
			// IsNullable(left) || IsNullable(right)
			if(n.left()->acceptVisitor(*this); !m_nullable) {
				n.right()->acceptVisitor(*this);
			}
		}

		void visit(const ConcatExpr& n) override {
			// IsNullable(left) && IsNullable(right)
			if(n.left()->acceptVisitor(*this); m_nullable) {
				n.right()->acceptVisitor(*this);
			}
		}


		bool m_nullable = {};
	};



	class RegularPattern::ComputeFirstPos : NodeVisitor {
	public:

		ComputeFirstPos(const RegularPattern& pattern, IndexList& posList) noexcept
			: m_pattern(pattern), m_posList(posList) {
		}

		void operator()(const ExprNode& n) {
			n.acceptVisitor(*this);
			if(IsNullable()(n)) {
				// add non-existent position to identify the end of a pattern
				m_posList.push_back(m_pattern.m_atoms.size());
			}
		}

	private:
		void visit(const CharAtom& n) override {
			m_posList.push_back(m_pattern.atomId(n));
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

		const RegularPattern& m_pattern;
		IndexList& m_posList;
	};



	class RegularPattern::ComputeFollowPos : NodeVisitor {
	public:

		ComputeFollowPos(const RegularPattern& pattern, IndexList& posList) noexcept
			: m_pattern(pattern), m_posList(posList) {
		}

		void operator()(const ExprNode& expr, const CharAtom& atom) {
			m_searchAtom = &atom;

			if(expr.acceptVisitor(*this); m_searchResult == SearchResult::Found) {
				m_posList.push_back(m_pattern.m_atoms.size());
			}
		}

	private:

		enum class SearchResult {
			Finished,
			Found,
			NotFound
		};


		void visit(const CharAtom& n) override {
			if(&n == m_searchAtom) {
				m_searchResult = SearchResult::Found;
			} else {
				m_searchResult = SearchResult::NotFound;
			}
		}

		void visit(const NilExpr&) override {
			m_searchResult = SearchResult::NotFound;
		}

		void visit(const OptionalExpr& n) override {
			n.inner()->acceptVisitor(*this);
		}

		void visit(const PlusExpr& n) override {
			n.inner()->acceptVisitor(*this);
			if(m_searchResult == SearchResult::Found) {
				ComputeFirstPos(m_pattern, m_posList)(*n.inner());
			}
		}

		void visit(const StarExpr& n) override {
			n.inner()->acceptVisitor(*this);
			if(m_searchResult == SearchResult::Found) {
				ComputeFirstPos(m_pattern, m_posList)(*n.inner());
			}
		}

		void visit(const AlternExpr& n) override {
			n.left()->acceptVisitor(*this);
			if(m_searchResult == SearchResult::NotFound) {
				n.right()->acceptVisitor(*this);
			}
		}

		void visit(const ConcatExpr& n) override {
			n.left()->acceptVisitor(*this);
			if(m_searchResult == SearchResult::Found) {
				ComputeFirstPos(m_pattern, m_posList)(*n.right());
				if(!IsNullable()(*n.right())) {
					m_searchResult = SearchResult::Finished;
				}
			} else if(m_searchResult == SearchResult::NotFound) {
				n.right()->acceptVisitor(*this);
			}
		}


		const RegularPattern& m_pattern;
		IndexList& m_posList;

		const CharAtom* m_searchAtom = {};
		SearchResult m_searchResult = {};
	};



	class RegularPattern::CollectAtomInfo : NodeTraverser {
	public:

		explicit CollectAtomInfo(RegularPattern& pattern) noexcept
			: m_pattern(pattern) {
		}

		void operator()(const ExprNode& n) {
			traverse(n);
		}

	private:
		void visit(const CharAtom& n) override {
			const auto atomId = m_pattern.m_atoms.size();

			// order is is important here to ensure m_atoms.size() gets updated first
			m_pattern.m_atoms.push_back(&n);
			m_pattern.m_atomIndex[&n] = atomId;
		}

		void visit(const NilExpr&) override {
			// nothing to do
		}

		RegularPattern& m_pattern;
	};



	RegularPattern::RegularPattern(std::string name, regex::RegularExpr regex, Id id)
		: m_regex(std::move(regex)), m_name(std::move(name)), m_id(id) {
		CollectAtomInfo(*this)(*m_regex.rootNode());
	}


	void RegularPattern::addExpr(regex::RegularExpr regex) {
		const auto oldAtomCount = m_atoms.size();
		try {
			CollectAtomInfo(*this)(*regex.rootNode());
			m_regex.altern(std::move(regex));
		} catch(...) {
			// cancel the effect of calling CollectAtomInfo()
			for(auto i = oldAtomCount; i < m_atoms.size(); i++) {
				m_atomIndex.erase(m_atoms.back());
				m_atoms.pop_back();
			}
			throw;
		}
	}


	IndexList RegularPattern::followPos(Index pos) const {
		IndexList posList;
		if(pos < m_atoms.size()) {
			ComputeFollowPos(*this, posList)(*m_regex.rootNode(), *m_atoms[pos]);
			
			const auto repeatElements = (std::ranges::sort(posList), std::ranges::unique(posList));
			posList.erase(repeatElements.begin(), repeatElements.end());
		}
		return posList;
	}


	IndexList RegularPattern::firstPos() const {
		IndexList posList;
		
		ComputeFirstPos(*this, posList)(*m_regex.rootNode());
		// positions found are guaranteed to be distinct and sorted in ascending order
		
		return posList;
	}


	std::optional<char> RegularPattern::charAt(Index pos) const {
		if(pos < m_atoms.size()) {
			return m_atoms[pos]->value();
		}
		return {};
	}
}