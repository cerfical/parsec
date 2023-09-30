#include "LexGenerator.hpp"
#include <boost/functional/hash.hpp>

namespace parsec {
	std::size_t LexGenerator::DfaStateHasher::operator()(const DfaState& state) const {
		return boost::hash_value(state);
	}
	
	void LexGenerator::RegisterEndMarkers() {
		// save the mappings between end markers and corresponding patterns
		for(const auto& pattern : *patterns) {
			endMarkers.emplace(&pattern.GetEndMarker(), &pattern);
		}
	}

	void LexGenerator::ComputeMatches(StateHandle state) {
		for(const auto ch : state->first) {
			if(IsEndMarker(ch)) {
				OnAccept(*endMarkers[ch]);
				return;
			}
		}
		OnReject();
	}


	LexGenerator::TransitionMap LexGenerator::ComputeNextStates(StateHandle state) {
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

	LexGenerator::DfaState LexGenerator::CreateStartState() {
		// combine firstpos sets of all patterns to get the start state of the DFA
		DfaState startState;
		for(const auto& pattern : *patterns) {
			const auto firstPos = pattern.GetRegex().GetFirstPos();
			startState.insert(firstPos.cbegin(), firstPos.cend());
		}
		return startState;
	}


	void LexGenerator::ProcessState(StateHandle state) {
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

	LexGenerator::StateHandle LexGenerator::CacheState(DfaState&& state) {
		const auto p = states.try_emplace(std::move(state), states.size());
		if(p.second) {
			unprocessedStates.push(&*p.first);
		}
		return &*p.first;
	}


	void LexGenerator::Generate() {
		if(patterns) {
			RegisterEndMarkers();
			CacheState(CreateStartState());
			while(!unprocessedStates.empty()) {
				const auto state = unprocessedStates.front();
				unprocessedStates.pop();
				ProcessState(state);
			}
		}
	}
}