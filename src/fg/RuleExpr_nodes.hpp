#ifndef PARSEC_FG_RULE_EXPR_NODES_HEADER
#define PARSEC_FG_RULE_EXPR_NODES_HEADER

#include "fg/RuleExpr.hpp"
#include <algorithm>

namespace parsec::fg {

	class RuleExpr::Symbol : public Node {
	public:

		void computeFirstPos(std::vector<std::size_t>& pos) const override {
			pos.push_back(posIndex);
		}

		SearchResult computeFollowPos(const Symbol* symbol, std::vector<std::size_t>&) const override {
			if(symbol == this) {
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
		std::size_t posIndex = 0;
	};



	class RuleExpr::Nil : public Node {
	public:

		void computeFirstPos(std::vector<std::size_t>&) const override {
			// nothing to do
		}

		SearchResult computeFollowPos(const Symbol*, std::vector<std::size_t>&) const override {
			return SearchResult::NotFound;
		}

		bool isNullable() const noexcept override {
			return true;
		}


		void printTo(std::ostream& out) const override {
			out << "()";
		}

	};



	class RuleExpr::Binary : public Node {
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



	class RuleExpr::Concat : public Binary {
	public:

		void computeFirstPos(std::vector<std::size_t>& pos) const override {
			left->computeFirstPos(pos);
			if(left->isNullable()) {
				right->computeFirstPos(pos);
			}
		}

		SearchResult computeFollowPos(const Symbol* symbol, std::vector<std::size_t>& pos) const override {
			const auto leftSearch = left->computeFollowPos(symbol, pos);
			if(leftSearch == SearchResult::Finished) {
				return leftSearch;
			}

			if(leftSearch == SearchResult::Found) {
				right->computeFirstPos(pos);
				if(right->isNullable()) {
					return SearchResult::Found;
				}
				return SearchResult::Finished;
			}

			return right->computeFollowPos(symbol, pos);
		}

		bool isNullable() const noexcept override {
			return left->isNullable() && right->isNullable();
		}


		void printTo(std::ostream& out) const override {
			printBinaryTo(" ", out);
		}

	};



	class RuleExpr::Altern: public Binary {
	public:

		void computeFirstPos(std::vector<std::size_t>& pos) const override {
			left->computeFirstPos(pos);
			right->computeFirstPos(pos);
		}

		SearchResult computeFollowPos(const Symbol* symbol, std::vector<std::size_t>& pos) const override {
			const auto leftSearch = left->computeFollowPos(symbol, pos);
			if(leftSearch != SearchResult::NotFound) {
				return leftSearch;
			}
			return right->computeFollowPos(symbol, pos);
		}

		bool isNullable() const noexcept override {
			return left->isNullable() || right->isNullable();
		}


		void printTo(std::ostream& out) const override {
			printBinaryTo(" | ", out);
		}

	};



	class RuleExpr::Repeat : public Node {
	public:
		
		void computeFirstPos(std::vector<std::size_t>& pos) const override {
			inner->computeFirstPos(pos);
		}

		SearchResult computeFollowPos(const Symbol* symbol, std::vector<std::size_t>& pos) const override {
			const auto innerSearch = inner->computeFollowPos(symbol, pos);
			if(innerSearch == SearchResult::Found) {
				inner->computeFirstPos(pos);
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



	class RuleExpr::EndSymbol : public Node {
	public:

		EndSymbol(const Node* rule, std::size_t posIndex)
			: m_posIndex(posIndex), m_rule(rule) {
		}


		void computeFirstPos(std::vector<std::size_t>& pos) const override {
			m_rule->computeFirstPos(pos);
			if(m_rule->isNullable()) {
				pos.push_back(m_posIndex);
			}
		}

		SearchResult computeFollowPos(const Symbol* symbol, std::vector<std::size_t>& pos) const override {
			const auto ruleSearch = m_rule->computeFollowPos(symbol, pos);
			if(ruleSearch == SearchResult::Found) {
				pos.push_back(m_posIndex);
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


		std::vector<std::size_t> followPosOf(const Symbol* symbol) const {
			std::vector<std::size_t> pos;

			computeFollowPos(symbol, pos);
			makeUnique(pos);
			
			return pos;
		}

		std::vector<std::size_t> firstPos() const {
			std::vector<std::size_t> pos;

			computeFirstPos(pos);
			// pos is guaranteed to contain unique position indices in ascending order

			return pos;
		}


	private:
		static void makeUnique(std::vector<std::size_t>& pos) {
			std::ranges::sort(pos);

			auto unique = std::ranges::unique(pos);
			pos.erase(unique.begin(), unique.end());
		}

		std::size_t m_posIndex = {};
		const Node* m_rule = {};
	};

}

#endif