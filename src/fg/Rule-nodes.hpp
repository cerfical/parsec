#ifndef PARSEC_FG_RULE_NODES_HEADER
#define PARSEC_FG_RULE_NODES_HEADER

#include "fg/Rule.hpp"
#include <algorithm>

namespace parsec::fg {

	class Rule::Symbol : public Node {
	public:

		void computeFirstPos(IndexList& posList) const override {
			posList.push_back(posIndex);
		}

		SearchResult computeFollowPos(const Symbol* pos, IndexList&) const override {
			if(pos == this) {
				return SearchResult::Found;
			}
			return SearchResult::NotFound;
		}

		bool isNullable() const noexcept override {
			return false;
		}


		void printTo(std::ostream& out) const override {
			out << name;
		}


		std::string name;
		Index posIndex = 0;
	};



	class Rule::Nil : public Node {
	public:

		void computeFirstPos(IndexList&) const override {
			// nothing to do
		}

		SearchResult computeFollowPos(const Symbol*, IndexList&) const override {
			return SearchResult::NotFound;
		}

		bool isNullable() const noexcept override {
			return true;
		}


		void printTo(std::ostream& out) const override {
			out << "()";
		}

	};



	class Rule::Binary : public Node {
	public:

		void printBinaryTo(std::string_view op, std::ostream& out) const {
			out << '(';
			left->printTo(out);
			out << op;
			right->printTo(out);
			out << ')';
		}

		NodePtr left;
		NodePtr right;
	};



	class Rule::Concat : public Binary {
	public:

		void computeFirstPos(IndexList& posList) const override {
			left->computeFirstPos(posList);
			if(left->isNullable()) {
				right->computeFirstPos(posList);
			}
		}

		SearchResult computeFollowPos(const Symbol* pos, IndexList& posList) const override {
			const auto leftSearch = left->computeFollowPos(pos, posList);
			if(leftSearch == SearchResult::Finished) {
				return leftSearch;
			}

			if(leftSearch == SearchResult::Found) {
				right->computeFirstPos(posList);
				if(right->isNullable()) {
					return SearchResult::Found;
				}
				return SearchResult::Finished;
			}

			return right->computeFollowPos(pos, posList);
		}

		bool isNullable() const noexcept override {
			return left->isNullable() && right->isNullable();
		}


		void printTo(std::ostream& out) const override {
			printBinaryTo(" ", out);
		}

	};



	class Rule::Altern: public Binary {
	public:

		void computeFirstPos(IndexList& posList) const override {
			left->computeFirstPos(posList);
			right->computeFirstPos(posList);
		}

		SearchResult computeFollowPos(const Symbol* pos, IndexList& posList) const override {
			const auto leftSearch = left->computeFollowPos(pos, posList);
			if(leftSearch != SearchResult::NotFound) {
				return leftSearch;
			}
			return right->computeFollowPos(pos, posList);
		}

		bool isNullable() const noexcept override {
			return left->isNullable() || right->isNullable();
		}


		void printTo(std::ostream& out) const override {
			printBinaryTo(" | ", out);
		}

	};



	class Rule::Repeat : public Node {
	public:
		
		void computeFirstPos(IndexList& posList) const override {
			inner->computeFirstPos(posList);
		}

		SearchResult computeFollowPos(const Symbol* pos, IndexList& posList) const override {
			const auto innerSearch = inner->computeFollowPos(pos, posList);
			if(innerSearch == SearchResult::Found) {
				inner->computeFirstPos(posList);
				return SearchResult::Found;
			}
			return innerSearch;
		}

		bool isNullable() const noexcept override {
			return false;
		}


		void printTo(std::ostream& out) const override {
			inner->printTo(out);
			out << '+';
		}


		NodePtr inner;
	};



	class Rule::EndSymbol : public Node {
	public:

		EndSymbol(const Node* rule, Index posIndex) noexcept
			: m_posIndex(posIndex), m_rule(rule) {
		}


		void computeFirstPos(IndexList& posList) const override {
			m_rule->computeFirstPos(posList);
			if(m_rule->isNullable()) {
				posList.push_back(m_posIndex);
			}
		}

		SearchResult computeFollowPos(const Symbol* pos, IndexList& posList) const override {
			const auto ruleSearch = m_rule->computeFollowPos(pos, posList);
			if(ruleSearch == SearchResult::Found) {
				posList.push_back(m_posIndex);
				return SearchResult::Finished;
			}
			return ruleSearch;
		}

		bool isNullable() const noexcept override {
			return m_rule->isNullable();
		}


		void printTo(std::ostream& out) const override {
			m_rule->printTo(out);
		}


		IndexList followPosOf(const Symbol* pos) const {
			IndexList posList;

			computeFollowPos(pos, posList);
			makeUnique(posList);
			
			return posList;
		}

		IndexList firstPos() const {
			IndexList posList;
			computeFirstPos(posList);
			// pos is guaranteed to contain unique position indices in ascending order

			return posList;
		}


	private:
		static void makeUnique(IndexList& posList) {
			const auto removedDuplicates = (std::ranges::sort(posList), std::ranges::unique(posList));
			posList.erase(removedDuplicates.begin(), removedDuplicates.end());
		}

		Index m_posIndex = {};
		const Node* m_rule = {};
	};

}

#endif