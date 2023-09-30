#ifndef PARSEC_LEX_AUTOMATON_HEADER
#define PARSEC_LEX_AUTOMATON_HEADER

#include "Utils.hpp"
#include "LexGrammar.hpp"

#include <vector>
#include <iostream>
#include <list>

namespace parsec {
	/**
	 * @brief DFA-based automaton to categorize character sequences from a continuous stream of characters.
	 */
	class LexAutomaton {
	public:
		class State;
		/**
		 * @brief Defines a relation between an input character and the new state of the automaton.
		 */
		class Transition {
		public:
			/** @{ */
			/** @brief Construct a new transition, conditioned on the specified character label. */
			Transition(const State& newState, char label) noexcept
			 : newState(&newState), label(label)
			{ }

			/** @copybrief */
			~Transition() = default;
			/** @} */

			/** @{ */
			Transition(const Transition&) = default;
			Transition& operator=(const Transition&) = default;
			/** @} */

			/** @{ */
			/** @brief Get a new state for the automaton after the transition. */
			const State& GetNewState() const noexcept {
				return *newState;
			}

			/** @brief Get the input character that will trigger the transition. */
			char GetLabel() const noexcept {
				return label;
			}
			/** @} */

		private:
			const State* newState;
			char label;
		};

		/** @brief List of @ref Transition "Transitions". */
		using TransitionList = std::vector<Transition>;


		/**
		 * @brief Reflects the input configuration of the automaton.
		 */
		class State {
		public:
			/** @{ */
			/** @brief Construct a new State with the assigned ID. */
			State(Uid id = 0) noexcept
			 : id(id)
			{ }

			/** @copybrief */
			~State() = default;
			/** @} */

			/** @{ */
			State(const State&) = default;
			State& operator=(const State&) = default;
			/** @} */

			/** @{ */
			State(State&&) = default;
			State& operator=(State&&) = default;
			/** @} */

			/** @{ */
			/** @brief Add a new out-transition to the state. */
			void AddTransition(const State& newState, char label) {
				transitions.emplace_back(newState, label);
			}

			/** @brief Add a new token to the list of tokens matched by the state. */
			void AddToken(const LexToken& tok) {
				tokens.push_back(tok);
			}
			/** @} */

			/** @{ */
			/** @brief Get the list of available transitions for the state. */
			const TransitionList& GetTransitions() const noexcept {
				return transitions;
			}

			/** @brief Get the number of transitions in the state. */
			std::size_t GetTransitionsCount() const noexcept {
				return transitions.size();
			}

			/** @brief Get the list of tokens matched by the state. */
			const LexTokenList& GetMatchedTokens() const noexcept {
				return tokens;
			}

			/** @brief Check if the state is accepting, i.e. has any tokens matched to the input. */
			bool IsAccepting() const noexcept {
				return !tokens.empty();
			}
			
			/** @brief Get the ID of the state. */
			Uid GetID() const noexcept {
				return id;
			}
			/** @} */

		private:
			TransitionList transitions;
			LexTokenList tokens;
			Uid id;
		};

		/** @brief List of @ref State "States". */
		using StateList = std::list<State>;


		/** @{ */
		/** @copybrief */
		LexAutomaton() = default;
		
		/** @brief Compile a new automaton from a grammar. */
		LexAutomaton(const LexGrammar& grammar);
		
		/** @copybrief */
		~LexAutomaton() = default;
		/** @} */


		/** @{ */
		LexAutomaton(LexAutomaton&&) = default;
		LexAutomaton& operator=(LexAutomaton&&) = default;
		/** @} */


		/** @{ */
		LexAutomaton(const LexAutomaton&) = delete;
		LexAutomaton& operator=(const LexAutomaton&) = delete;
		/** @} */


		/** @{ */
		/** @brief Set a new start state for the automaton. */
		void SetStartState(const State& state) {
			startState = &state;
		}

		/** @brief Get the start state of the automaton. */
		const State& GetStartState() const noexcept {
			return *startState;
		}
		/** @} */


		/** @{ */
		/** @brief Allocate a new empty state in the automaton. */
		State& AllocState() {
			return states.emplace_back(states.size());
		}

		/** @brief Get the list of all states of the automaton. */
		const StateList& GetStates() const noexcept {
			return states;
		}

		/** @brief Get the number of states in the automaton. */
		std::size_t GetStatesCount() const noexcept {
			return states.size();
		}
		/** @} */

		/** @{ */
		/** @brief Print out all information about the automaton to a @c std::ostream. */
		void Dump(std::size_t indent = 0, std::ostream& out = std::cout);
		/** @} */

	private:
		static State emptyState;

		const State* startState = &emptyState;
		StateList states;
	};
}

#endif