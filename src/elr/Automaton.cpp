#include "elr/Automaton.hpp"

#include "core/RuleConflictError.hpp"
#include "dfa/Automaton.hpp"

#include <boost/functional/hash.hpp>

#include <map>
#include <set>
#include <unordered_map>

namespace parsec::elr {
	namespace {
		class StateItem {
		public:

			friend auto operator<=>(const StateItem&, const StateItem&) noexcept = default;


			StateItem(int dfaState, int backLink = -1) noexcept
				: m_dfaState(dfaState), m_backLink(backLink) {}


			int dfaState() const noexcept {
				return m_dfaState;
			}


			int backLink() const noexcept {
				return m_backLink;
			}


		private:
			int m_backLink = {};
			int m_dfaState = {};
		};


		using ItemSet = std::set<StateItem>;


		class TransNetwork : private NonCopyable {
		public:

			TransNetwork() = default;

			explicit TransNetwork(const SymbolGrammar& grammar) {
				for(const auto& rule : grammar.rules()) {
					// skip empty rules
					if(!rule) {
						continue;
					}

					insertMachine(rule.head(), dfa::Automaton(rule));
				}
			}


			const dfa::State* stateById(int state) const noexcept {
				return &m_states[state];
			}


			const dfa::State* startState(const Symbol& symbol) const noexcept {
				const auto symbolToIndexIt = m_symbolToStartState.find(symbol);
				if(symbolToIndexIt != m_symbolToStartState.end()) {
					return &m_states[symbolToIndexIt->second];
				}
				return nullptr;
			}


			const std::vector<dfa::State>& states() const noexcept {
				return m_states;
			}


			explicit operator bool() const noexcept {
				return !isEmpty();
			}


			bool isEmpty() const noexcept {
				return m_states.empty();
			}


		private:
			void insertMachine(const Symbol& name, const dfa::Automaton& dfa) {
				const auto baseStateId = static_cast<int>(m_states.size());
				for(const auto& dfaState : dfa.states()) {
					// create a new state with a unique id for each DFA state
					auto& state = m_states.emplace_back(baseStateId + dfaState.id());
					for(const auto& dfaTrans : dfaState.transitions()) {
						state.addTransition(
							baseStateId + dfaTrans.target,
							dfaTrans.label
						);
					}
					state.setMatchedRule(dfaState.matchedRule());
				}

				const auto startState = baseStateId + dfa.startState().id();
				m_symbolToStartState[name] = startState;
				m_states[startState].setStartState(true);
			}


			struct SymbolHasher {
				std::size_t operator()(const Symbol& symbol) const noexcept {
					return hash(symbol);
				}
			};

			std::vector<dfa::State> m_states;
			std::unordered_map<Symbol, int, SymbolHasher> m_symbolToStartState;
		};


		const State emptyState;
	}
}


template <>
struct boost::hash<parsec::elr::StateItem> {
	std::size_t operator()(const parsec::elr::StateItem& item) const noexcept {
		return boost::hash_value(std::tuple(item.dfaState(), item.backLink()));
	}
};


namespace parsec::elr {
	class Automaton::StateBuilder {
	public:

		StateBuilder(const SymbolGrammar& grammar, std::vector<State>& states)
			: m_grammar(grammar), m_states(states) {}


		void run() {
			m_transNet = TransNetwork(m_grammar);
			if(const auto startState = createStartState(); !startState.empty()) {
				const auto startStateId = buildState(startState);
				m_states[startStateId].setStartState(true);
			}
		}


	private:
		ItemSet createStartState() {
			ItemSet startState;
			if(const auto& root = m_grammar.root()) {
				startState.insert(m_transNet.startState(root.head())->id());
			}
			return startState;
		}


		int buildState(const ItemSet& stateItems) {
			const auto [it, wasInserted] = m_itemSetsToIds.try_emplace(
				closure(stateItems),
				static_cast<int>(m_itemSetsToIds.size())
			);
			const auto& [items, stateId] = *it;

			if(wasInserted) {
				auto& state = m_states.emplace_back(stateId);
				for(const auto& item : items) {
					state.addBackLink(item.backLink());
				}

				buildTransitions(items, stateId);
			}
			return stateId;
		}


		ItemSet closure(const ItemSet& items) const {
			ItemSet closure;
			for(const auto& item : items) {
				closeItem(item, closure);
			}
			return closure;
		}


		void closeItem(const StateItem& item, ItemSet& closure) const {
			if(closure.contains(item)) {
				return;
			}
			closure.insert(item);

			for(const auto& trans : m_transNet.stateById(item.dfaState())->transitions()) {
				if(const auto startState = m_transNet.startState(trans.label)) {
					closeItem(startState->id(), closure);
				}
			}
		}


		void buildTransitions(const ItemSet& items, int stateId) {
			// use map to lexicographically sort transitions by their labels
			std::map<Symbol, ItemSet> transitions;
			
			for(int itemId = 0; const auto& item : items) {
				const auto& dfaState = m_transNet.stateById(item.dfaState());
				if(dfaState->isMatchState()) {
					if(!m_states[stateId].isReduceState()) {
						m_states[stateId].setReduction(dfaState->matchedRule(), itemId);
					} else {
						throw RuleConflictError(
							m_states[stateId].reduction().reduceRule,
							dfaState->matchedRule()
						);
					}
				}
				
				for(const auto& trans : dfaState->transitions()) {
					transitions[trans.label].insert(StateItem(trans.target, itemId));
				}

				itemId++;
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


		std::unordered_map<ItemSet, int, boost::hash<ItemSet>> m_itemSetsToIds;
		TransNetwork m_transNet;

		const SymbolGrammar& m_grammar;
		std::vector<State>& m_states;
	};


	Automaton::Automaton(const SymbolGrammar& grammar) {
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