#ifndef PARSEC_FSM_AUTOMATON_HEADER
#define PARSEC_FSM_AUTOMATON_HEADER

#include "../core/NonCopyable.hpp"
#include "State.hpp"

#include <vector>
#include <unordered_map>
#include <span>

namespace parsec::fsm {

	class StateMachine : private NonCopyable {
	public:

		void buildTransition(int from, int to, const fg::Symbol& label);


		std::span<const State> states() const;;

		const State& stateById(int state) const;


		void markAcceptState(int state, const fg::Symbol& inputMatch);
		
		void setStartState(int state) {
			m_startState = state;
		}
		
		const State& startState() const {
			return stateById(m_startState);
		}


		explicit operator bool() const {
			return !isEmpty();
		}

		bool isEmpty() const {
			return m_states.empty();
		}


	private:
		const State& stateByIndex(std::size_t i) const noexcept {
			return m_states[i];
		}

		State& stateByIndex(std::size_t i) noexcept {
			return m_states[i];
		}

		std::size_t insertState(int state);
		void eraseState(int state) noexcept;
		
		void updateCache() const;

		mutable std::vector<State> m_states;
		mutable std::unordered_map<int, std::size_t> m_idToIndexCache;
		mutable bool m_statesSorted = true;

		int m_startState = {};
	};

}

#endif