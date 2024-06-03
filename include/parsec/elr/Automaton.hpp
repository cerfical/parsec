#pragma once

#include "../utils/util_types.hpp"
#include "../core/SymbolGrammar.hpp"
#include "State.hpp"

#include <vector>

namespace parsec::elr {

	class Automaton : private NonCopyable {
	public:

		Automaton() noexcept = default;

		explicit Automaton(const SymbolGrammar& grammar);


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
