#include "elr/TransNetwork.hpp"

namespace parsec::elr {
	namespace {
		const dfa::State emptyState;
	}


	TransNetwork::TransNetwork(const SymbolGrammar& grammar) {
		for(const auto& rule : grammar.rules()) {
			// skip empty rules
			if(!rule) {
				continue;
			}

			SymbolGrammar singletonGrammar;
			singletonGrammar.define(rule.head(), rule.body());

			insertMachine(rule.head(), dfa::Automaton(singletonGrammar));
		}
	}


	void TransNetwork::insertMachine(const Symbol& name, const dfa::Automaton& dfa) {
		// function to be called only in constructors, for exception safety
		
		const auto baseStateId = static_cast<int>(m_states.size());
		for(const auto& dfaState : dfa.states()) {
			// create a new state with a unique id for each DFA state
			auto& state = m_states.emplace_back(baseStateId + dfaState.id());
			for(const auto& dfaTrans : dfaState.transitions()) {
				state.addTransition(
					baseStateId + dfaTrans.target(),
					dfaTrans.label()
				);
			}
			state.setMatchedRule(dfaState.matchedRule());
		}

		const auto startState = baseStateId + dfa.startState().id();
		m_symbolToStartState[name] = startState;
		m_states[startState].setStartState(true);
	}


	const dfa::State& TransNetwork::startState(const Symbol& symbol) const {
		const auto symbolToIndexIt = m_symbolToStartState.find(symbol);
		if(symbolToIndexIt != m_symbolToStartState.end()) {
			return m_states[symbolToIndexIt->second];
		}
		return emptyState;
	}


	const dfa::State& TransNetwork::stateById(int state) const {
		if(state < m_states.size()) {
			return m_states[state];
		}
		return emptyState;
	}
}