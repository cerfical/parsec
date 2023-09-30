#include "LexGen.hpp"
#include <boost/functional/hash.hpp>

namespace parsec {
	std::size_t LexGen::StateHasher::operator()(const State& state) const {
		return boost::hash_value(state);
	}
	
	void LexGen::RegisterEndMarkers() {
		// save the mappings between end markers and corresponding patterns
		for(const auto& tok : m_grammar->GetTokens()) {
			m_endMarkers.emplace(tok.GetEndMarker(), &tok);
		}
	}

	void LexGen::ComputeMatches(StateHandle state) {
		const TokenPattern* match = nullptr;
		for(const auto ch : state->first) {
			if(IsEndMarker(ch)) {
				const auto newMatch = m_endMarkers[ch];
				if(!match || newMatch->GetPriority() <= match->GetPriority()) {
					match = newMatch;
				}
			}
		}
		if(match) {
			OnAccept(*match);
		} else {
			OnReject();
		}
	}

	LexGen::TransitionMap LexGen::ComputeNextStates(StateHandle state) {
		TransitionMap nextStates;
		for(const auto ch : state->first) {
			if(!IsEndMarker(ch)) {
				const auto followPos = ch->GetFollowPos();
				nextStates[ch->GetValue()].insert(
					followPos.cbegin(), followPos.cend()
				);
			}
		}
		return nextStates;
	}

	LexGen::State LexGen::CreateStartState() {
		// combine firstpos sets of all patterns to get the start state of the DFA
		State startState;
		for(const auto& tok : m_grammar->GetTokens()) {
			const auto firstPos = tok.GetRegex()->GetFirstPos();
			startState.insert(firstPos.cbegin(), firstPos.cend());
		}
		return startState;
	}

	void LexGen::ProcessState(StateHandle state) {
		OnPreState(state->second);
		auto transitions = ComputeNextStates(state);
		if(!transitions.empty()) {
			OnPreTransitions();
			for(auto& trans : transitions) {
				const auto newState = CacheState(std::move(trans.second));
				OnTransition(newState->second, trans.first);
			}
			OnPostTransitions();
		}
		ComputeMatches(state);
		OnPostState(state->second);
	}

	LexGen::StateHandle LexGen::CacheState(State&& state) {
		const auto p = m_states.try_emplace(std::move(state), gsl::narrow_cast<StateID>(m_states.size()));
		// only newly added states need to be processed
		if(p.second) {
			m_unprocessedStates.push(&*p.first);
		}
		return &*p.first;
	}

	void LexGen::Gen() {
		if(m_grammar) {
			RegisterEndMarkers();
			CacheState(CreateStartState());
			while(!m_unprocessedStates.empty()) {
				const auto state = m_unprocessedStates.front();
				m_unprocessedStates.pop();
				ProcessState(state);
			}
		}
	}
}