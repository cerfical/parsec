#ifndef PARSEC_DFA_AUTOMATON_HEADER
#define PARSEC_DFA_AUTOMATON_HEADER

#include "../core/SymbolGrammar.hpp"
#include "State.hpp"

#include <vector>
#include <span>

namespace parsec::dfa {

	class Automaton : private NonCopyable {
	public:

		Automaton() = default;

		explicit Automaton(const SymbolGrammar& grammar);


		std::span<const State> states() const {
			return m_states;
		}

		const State& stateById(int state) const;

		const State& startState() const;


		explicit operator bool() const {
			return !isEmpty();
		}

		bool isEmpty() const {
			return m_states.empty();
		}


	private:
		class StateBuilder;
		using StateList = std::vector<State>;

		StateList m_states;
	};

}

#endif