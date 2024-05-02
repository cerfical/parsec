#ifndef PARSEC_DFA_AUTOMATON_HEADER
#define PARSEC_DFA_AUTOMATON_HEADER

#include "../core/SymbolGrammar.hpp"
#include "State.hpp"

#include <vector>

namespace parsec::dfa {

	class Automaton : private NonCopyable {
	public:

		Automaton() noexcept = default;

		explicit Automaton(const SymbolGrammar& grammar);

		explicit Automaton(const SymbolRule& pattern);


		const std::vector<State>& states() const noexcept {
			return m_states;
		}


		const State& stateById(int state) const noexcept;


		const State& startState() const noexcept;


		explicit operator bool() const noexcept {
			return !isEmpty();
		}


		bool isEmpty() const noexcept {
			return m_states.empty();
		}


	private:
		class StateBuilder;

		std::vector<State> m_states;
	};

}

#endif