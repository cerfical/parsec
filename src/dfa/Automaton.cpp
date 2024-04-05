#include "dfa/Automaton.hpp"

#include "core/RuleConflictError.hpp"
#include "hash.hpp"

#include <boost/functional/hash.hpp>
#include <unordered_set>
#include <map>

template <>
struct boost::hash<parsec::Symbol> {
	std::size_t operator()(const parsec::Symbol& symbol) {
		return parsec::hash(symbol);
	}
};

namespace parsec::dfa {
	namespace {
		class Item {
		public:

			Item(const SymbolRule* rule, int pos)
				: m_rule(rule), m_pos(pos) {}


			const SymbolRule* rule() const {
				return m_rule;
			}

			const Symbol& value() const {
				return m_rule->body().posValue(m_pos);
			}

			int pos() const {
				return m_pos;
			}

			bool isAtEnd() const {
				return m_rule->body().isEndPos(m_pos);
			}


		private:
			const SymbolRule* m_rule = {};
			int m_pos = {};
		};

		using ItemSet = std::unordered_set<Item, boost::hash<Item>>;


		inline bool operator==(const Item& lhs, const Item& rhs) {
			return std::tuple(lhs.rule()->head(), lhs.pos())
				== std::tuple(rhs.rule()->head(), rhs.pos());
		}

		inline std::size_t hash_value(const Item& item) {
			return boost::hash_value(std::tuple(item.rule()->head(), item.pos()));
		}


		const State emptyState;
	}


	class Automaton::StateBuilder {
	public:

		StateBuilder(const SymbolGrammar& grammar, StateList& states)
			: m_grammar(grammar), m_states(states) {}

		void run() {
			if(auto startState = createStartState(); !startState.empty()) {
				const auto startStateId = buildState(std::move(startState));
				m_states[startStateId].setStartState(true);
			}
		}

	private:
		ItemSet createStartState() const {
			ItemSet items;
			for(const auto& rule : m_grammar.rules()) {
				for(const auto& pos : rule.body().firstPos()) {
					items.emplace(&rule, pos);
				}
			}
			return items;
		}

		int buildState(ItemSet&& stateItems) {
			const auto [it, wasInserted] = m_itemSetsToIds.emplace(
				std::move(stateItems),
				static_cast<int>(m_itemSetsToIds.size())
			);
			const auto& [itemSet, stateId] = *it;

			if(wasInserted) {
				m_states.emplace_back(stateId);
				buildTransitions(itemSet, stateId);
			}
			return stateId;
		}

		void buildTransitions(const ItemSet& stateItems, int stateId) {
			// use map to lexicographically sort transitions by their labels
			std::map<Symbol, ItemSet> transitions;
			
			for(const auto& item : stateItems) {
				if(item.isAtEnd()) {
					if(!m_states[stateId].isMatchState()) {
						m_states[stateId].setMatchedRule(item.rule()->head());
						continue;
					}
					throw RuleConflictError(m_states[stateId].matchedRule(), item.rule()->head());
				}

				auto& itemTrans = transitions[item.value()];
				for(const auto& pos : item.rule()->body().followPos(item.pos())) {
					itemTrans.emplace(item.rule(), pos);
				}
			}

			for(auto& [transLabel, transTarget] : transitions) {
				const auto transTargetId = buildState(std::move(transTarget));
				m_states[stateId].addTransition(transTargetId, transLabel);
			}
		}

		std::unordered_map<ItemSet, int, boost::hash<ItemSet>> m_itemSetsToIds;

		const SymbolGrammar& m_grammar;
		StateList& m_states;
	};


	Automaton::Automaton(const SymbolGrammar& grammar) {
		StateBuilder(grammar, m_states).run();
	}


	const State& Automaton::stateById(int state) const {
		if(state < m_states.size()) {
			return m_states[state];
		}
		return emptyState;
	}


	const State& Automaton::startState() const {
		if(!m_states.empty()) {
			return m_states.front();
		}
		return emptyState;
	}
}