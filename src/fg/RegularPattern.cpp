#include "fg/RegularPattern.hpp"

#include "regex/nodes.hpp"
#include <algorithm>

using PosList = std::vector<std::size_t>;

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

		ComputeFirstPos(const RegularPattern& pattern, PosList& pos) noexcept
			: m_pattern(pattern), m_pos(pos) {
		}

		void operator()(const ExprNode& n) {
			n.acceptVisitor(*this);
			if(IsNullable()(n)) {
				// add non-existent position to identify the end of a pattern
				m_pos.push_back(m_pattern.m_atoms.size());
			}
		}

	private:
		void visit(const CharAtom& n) override {
			m_pos.push_back(m_pattern.atomId(n));
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
		PosList& m_pos;
	};



	class RegularPattern::ComputeFollowPos : NodeVisitor {
	public:

		ComputeFollowPos(const RegularPattern& pattern, PosList& pos) noexcept
			: m_pattern(pattern), m_pos(pos) {
		}

		void operator()(const ExprNode& expr, const CharAtom& atom) {
			m_searchAtom = &atom;

			if(expr.acceptVisitor(*this); m_searchResult == SearchResult::Found) {
				m_pos.push_back(m_pattern.m_atoms.size());
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
				ComputeFirstPos(m_pattern, m_pos)(*n.inner());
			}
		}

		void visit(const StarExpr& n) override {
			n.inner()->acceptVisitor(*this);
			if(m_searchResult == SearchResult::Found) {
				ComputeFirstPos(m_pattern, m_pos)(*n.inner());
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
				ComputeFirstPos(m_pattern, m_pos)(*n.right());
				if(!IsNullable()(*n.right())) {
					m_searchResult = SearchResult::Finished;
				}
			} else if(m_searchResult == SearchResult::NotFound) {
				n.right()->acceptVisitor(*this);
			}
		}


		const RegularPattern& m_pattern;
		PosList& m_pos;

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
			m_pattern.m_atomIndex[&n] = m_pattern.m_atoms.size();
			m_pattern.m_atoms.push_back(&n);
		}

		void visit(const NilExpr&) override {
			// nothing to do
		}

		RegularPattern& m_pattern;
	};



	RegularPattern::RegularPattern(std::string name, regex::RegularExpr regex, int id)
		: m_regex(std::move(regex)), m_name(std::move(name)), m_id(id) {
		CollectAtomInfo(*this)(*m_regex.rootNode());
	}


	std::vector<std::size_t> RegularPattern::followPos(std::size_t i) const {
		PosList pos;
		if(i < m_atoms.size()) {
			ComputeFollowPos(*this, pos)(*m_regex.rootNode(), *m_atoms[i]);
			
			const auto repeatElements = (std::ranges::sort(pos), std::ranges::unique(pos));
			pos.erase(repeatElements.begin(), repeatElements.end());
		}
		return pos;
	}

	std::vector<std::size_t> RegularPattern::firstPos() const {
		PosList pos;
		
		ComputeFirstPos(*this, pos)(*m_regex.rootNode());
		// positions found are guaranteed to be distinct and sorted in ascending order
		
		return pos;
	}

	std::optional<char> RegularPattern::charAt(std::size_t i) const {
		if(i < m_atoms.size()) {
			return m_atoms[i]->value();
		}
		return {};
	}
}