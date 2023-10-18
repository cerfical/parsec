#include "gen/LexerGenerator.hpp"
#include "regex/nodes.hpp"

#include <boost/functional/hash.hpp>
#include <gsl/gsl>

namespace parsec::gen {
	std::size_t LexerGenerator::StateHasher::operator()(const State& state) const {
		return boost::hash_value(state);
	}

	
	const fg::TokenRule* LexerGenerator::computeRuleMatch(StateHandle state) const {
		const fg::TokenRule* match = nullptr;
		for(const auto ch : state->first) {
			if(isEndMarker(ch)) {
				const auto newMatch = m_endMarkers.find(ch)->second;
				if(!match || newMatch->priority() < match->priority()) {
					match = newMatch;
				}
			}
		}
		return match;
	}

	LexerGenerator::TransitionMap LexerGenerator::computeTransitions(StateHandle state) const {
		TransitionMap transitions;
		for(const auto ch : state->first) {
			if(!isEndMarker(ch)) {
				const auto followPos = ch->followPos();
				transitions[ch->value()].insert(
					followPos.cbegin(), followPos.cend()
				);
			}
		}
		return transitions;
	}


	LexerGenerator::State LexerGenerator::createStartState() const {
		// combine firstpos sets of all rules to get the start state of the DFA
		State startState;
		for(const auto& rule : m_grammar->tokens()) {
			const auto firstPos = rule.pattern()->firstPos();
			startState.insert(firstPos.cbegin(), firstPos.cend());
		}
		return startState;
	}

	void LexerGenerator::registerEndMarkers() {
		// save the mappings between end markers and corresponding rules
		for(const auto& rule : m_grammar->tokens()) {
			m_endMarkers.emplace(rule.endMarker(), &rule);
		}
	}


	void LexerGenerator::processState(StateHandle stateHandle) {
		const auto match = computeRuleMatch(stateHandle);
		auto transitions = computeTransitions(stateHandle);

		m_lexerState = LexerState(stateHandle->second, match);
		for(auto& trans : transitions) {
			const auto newState = cacheState(std::move(trans.second));
			m_lexerState.addTransition(newState->second, trans.first);
		}

		onLexerState(m_lexerState);
	}

	LexerGenerator::StateHandle LexerGenerator::cacheState(State&& state) {
		const auto p = m_dfaStates.try_emplace(std::move(state), gsl::narrow_cast<int>(m_dfaStates.size()));
		// only newly added states need to be processed
		if(p.second) {
			m_unprocessedStates.push(&*p.first);
		}
		return &*p.first;
	}


	void LexerGenerator::generateStates() {
		registerEndMarkers();
		cacheState(createStartState());
		while(!m_unprocessedStates.empty()) {
			const auto state = m_unprocessedStates.front();
			m_unprocessedStates.pop();
			processState(state);
		}
	}

	void LexerGenerator::compile(const fg::Grammar& grammar) {
		// reset the input grammar once the generation is complete
		const auto sentry = gsl::finally([this]() {
			m_grammar = nullptr;
		});
		m_grammar = &grammar;

		onCompile();
	}
}