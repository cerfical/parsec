#include "elr/Automaton.hpp"

#include "elr/TransNetwork.hpp"
#include "elr/ItemSet.hpp"

#include <unordered_map>
#include <map>

namespace parsec::elr {
	namespace {
		const State emptyState;
	}


	class Automaton::StateBuilder {
	public:

		StateBuilder(const fg::SymbolGrammar& grammar, StateList& states)
			: m_grammar(grammar), m_states(states) {}

		void run() {
			m_transNet = TransNetwork(m_grammar);
			if(const auto startStateItems = createStartState(); startStateItems) {
				const auto startStateId = buildState(startStateItems);
				m_states[startStateId].setStartState(true);
			}
		}

	private:
		ItemSet createStartState() {
			ItemSet startState;
			startState.insert(
				m_transNet.startState(m_grammar.root().head()).id()
			);
			return startState;
		}

		int buildState(const ItemSet& stateItems) {
			const auto [it, wasInserted] = m_itemSetsToIds.try_emplace(
				stateItems.closure(m_transNet),
				static_cast<int>(m_itemSetsToIds.size())
			);
			const auto& [itemSet, stateId] = *it;

			if(wasInserted) {
				auto& state = m_states.emplace_back(stateId);
				for(const auto& item : itemSet.items()) {
					state.addBackLink(item.backLink());
				}
				buildTransitions(itemSet.items(), stateId);
			}
			return stateId;
		}

		void buildTransitions(std::span<const StateItem> stateItems, int stateId) {
			// use map to lexicographically sort transitions by their labels
			std::map<fg::Symbol, ItemSet> transitions;
			
			for(std::size_t i = 0; i < stateItems.size(); i++) {
				const auto& dfaState = m_transNet.stateById(stateItems[i].dfaState());
				if(dfaState.isMatchState()) {
					if(!m_states[stateId].isReduceState()) {
						m_states[stateId].setReduction(dfaState.matchedRule(), i);
					} else {
						throw std::runtime_error("conflicting rules");
					}
				}
				
				for(const auto& trans : dfaState.transitions()) {
					transitions[trans.label()].insert(StateItem(trans.target(), i));
				}
			}

			for(const auto& [transLabel, transTarget] : transitions) {
				const auto transTargetId = buildState(transTarget);

				// check whether the symbol is a nonterminal symbol
				if(m_grammar.contains(transLabel)) {
					m_states[stateId].addGoto(transTargetId, transLabel);
				} else {
					m_states[stateId].addShift(transTargetId, transLabel);
				}
			}
		}


		struct ItemSetHasher {
			std::size_t operator()(const ItemSet& itemSet) const noexcept {
				return hash(itemSet);
			}
		};


		std::unordered_map<ItemSet, int, ItemSetHasher> m_itemSetsToIds;
		TransNetwork m_transNet;

		const fg::SymbolGrammar& m_grammar;
		StateList& m_states;
	};


	Automaton::Automaton(const fg::SymbolGrammar& grammar) {
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