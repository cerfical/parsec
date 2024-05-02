#include "dfa/Automaton.hpp"

#include "core/RuleConflictError.hpp"
#include "hash.hpp"

#include <boost/functional/hash.hpp>

#include <unordered_set>
#include <unordered_map>
#include <map>

namespace parsec::dfa {
	namespace {
		class StateItem {
		public:

			StateItem(const SymbolRule* rule, int pos) noexcept
				: m_rule(rule), m_pos(pos) {}


			const SymbolRule* rule() const noexcept {
				return m_rule;
			}


			int pos() const noexcept {
				return m_pos;
			}


			const Symbol& value() const {
				return m_rule->body().posValue(m_pos);
			}


			bool isAtEnd() const {
				return m_rule->body().isEndPos(m_pos);
			}


		private:
			const SymbolRule* m_rule = {};
			int m_pos = {};
		};


		using ItemSet = std::unordered_set<StateItem, boost::hash<StateItem>>;


		inline bool operator==(const StateItem& lhs, const StateItem& rhs) noexcept {
			return std::tuple(lhs.rule()->head(), lhs.pos())
				== std::tuple(rhs.rule()->head(), rhs.pos());
		}


		const State emptyState;
	}
}


template <>
struct boost::hash<parsec::Symbol> {
	std::size_t operator()(const parsec::Symbol& symbol) const noexcept {
		return parsec::hash(symbol);
	}
};


template <>
struct boost::hash<parsec::dfa::StateItem> {
	std::size_t operator()(const parsec::dfa::StateItem& item) const noexcept {
		return boost::hash_value(std::tuple(item.rule()->head(), item.pos()));
	}
};


namespace parsec::dfa {
	class Automaton::StateBuilder {
	public:

		StateBuilder(const SymbolGrammar& grammar, std::vector<State>& states)
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
				std::move(stateItems), static_cast<int>(m_itemSetsToIds.size())
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
		std::vector<State>& m_states;
	};


	Automaton::Automaton(const SymbolGrammar& grammar) {
		StateBuilder(grammar, m_states).run();
	}


	Automaton::Automaton(const SymbolRule& pattern) {
		// create a singleton grammar containing the pattern as the only defined symbol
		SymbolGrammar grammar;
		grammar.define(pattern.head(), pattern.body());
		
		StateBuilder(grammar, m_states).run();
	}


	const State& Automaton::stateById(int state) const noexcept {
		if(state >= 0 && state < m_states.size()) {
			return m_states[state];
		}
		return emptyState;
	}


	const State& Automaton::startState() const noexcept {
		if(!m_states.empty()) {
			return m_states.front();
		}
		return emptyState;
	}
}