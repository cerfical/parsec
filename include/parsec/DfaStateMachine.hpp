#ifndef PARSEC_DFA_STATE_MACHINE_HEADER
#define PARSEC_DFA_STATE_MACHINE_HEADER

#include "Utils.hpp"

#include <iostream>
#include <compare>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace parsec {
	/**
	 * @brief Deterministic finite automaton implementation.
	 */
	class DfaStateMachine {
	public: class Position;
	private:
		struct PositionHasher {
			std::size_t operator()(const Position& pos) const noexcept;
		};

	public:
		/**
		 * @brief A set of unique @ref Position "Positions".
		 */
		using PositionSet = std::unordered_set<Position, PositionHasher>;

	private:
		struct PositionSetHasher {
			std::size_t operator()(const PositionSet& positions) const;
		};
		using StateIdsMap = std::unordered_map<PositionSet, Uid, PositionSetHasher>;

	public:
		/**
		 * @brief Represents a single occurrence of a literal character in a regular expression.
		 */
		class Position {
		public:
			/** @brief Compare two @ref Position "Positions". */
			friend auto operator<=>(const Position& lhs, const Position& rhs) = default;

			/** @brief Print out a Position to a @c std::ostream. */
			friend std::ostream& operator<<(std::ostream& out, const Position& pos);

			/** @{ */
			/** @brief Construct a new position from a character and a unique identifier. */
			Position(char ch, Uid id = 0) noexcept
			 : ch(ch), id(id)
			{ }

			/** @brief Construct a new end marker position with a unique identifier. */
			Position(Uid id = 0) noexcept
			 : id(id)
			{ }

			/** @copybrief */
			~Position() = default;
			/** @} */

			/** @{ */
			Position(const Position&) = default;
			Position& operator=(const Position&) = default;
			/** @} */

			/** @{ */
			/** @brief Get the character associated with the position. */
			char GetChar() const noexcept {
				if(ch) {
					return *ch;
				}
				return '#';
			}

			/** @brief Get the position's identifier. */
			Uid GetID() const noexcept {
				return id;
			}

			/** @brief Check if the position defines an end marker. */
			bool IsEndMarker() const noexcept {
				return !ch.has_value();
			}
			/** @} */

		private:
			std::optional<char> ch;
			Uid id;
		};


		/**
		 * @brief DFA state as a set of positions in some regular expression.
		 */
		class State {
			friend DfaStateMachine;

		public:
			/** @brief Compare two @ref State "States" for equality. */
			friend bool operator==(const State& lhs, const State& rhs) = default;

			/** @brief Print out a State to a @c std::ostream. */
			friend std::ostream& operator<<(std::ostream& out, const State& state);

			/** @{ */
			/** @copybrief */
			State() = default;
			
			/** @copybrief */
			~State() = default;
			/** @} */

			/** @{ */
			State(const State&) = default;
			State& operator=(const State&) = default;
			/** @} */

			/** @{ */
			/** @brief Get the state's associated set of positions. */
			const PositionSet& GetPositions() const noexcept {
				return state->first;
			}

			/** @brief Get the unique identifier for the state. */
			Uid GetID() const noexcept {
				return state->second;
			}
			/** @} */

		private:
			static StateIdsMap::value_type emptyState;

			State(const StateIdsMap::value_type& state) noexcept
			 : state(&state)
			{ }

			const StateIdsMap::value_type* state = &emptyState;
		};

		/** @brief A list of @ref State "States". */
		using StateList = std::vector<State>;


		/**
		 * @brief Defines a transition between two states on the condition that a certain character was encountered.
		 */
		class StateTransition {
		public:
			/** @brief Compare two state transitions for equality. */
			friend bool operator==(const StateTransition& lhs, const StateTransition& rhs) = default;
			
			/** @brief Print out a StateTransition to a @c std::ostream. */
			friend std::ostream& operator<<(std::ostream& out, const StateTransition& trans);

			/** @{ */
			/** @brief Construct a new labeled transition from one state to another. */
			StateTransition(const State& from, const State& to, char label) noexcept
			 : from(from), to(to), label(label)
			{ }

			/** @copybrief */
			~StateTransition() = default;
			/** @} */

			/** @{ */
			StateTransition(const StateTransition&) = default;
			StateTransition& operator=(const StateTransition&) = default;
			/** @} */

			/** @{ */
			/** @brief Get the "from" state of the transition. */
			const State& GetFromState() const noexcept {
				return from;
			}

			/** @brief Get the "to" state of the transition. */
			const State& GetToState() const noexcept {
				return to;
			}

			/** @brief Get the label of the transition. */
			char GetLabel() const noexcept {
				return label;
			}
			/** @} */

		private:
			State from;
			State to;

			char label;
		};

		/** @brief A list of @ref StateTransition "StateTransitions". */
		using TransitionList = std::vector<StateTransition>;


		/** @{ */
		/** @copybrief */
		DfaStateMachine() = default;

		/** @copybrief */
		~DfaStateMachine() = default;
		/** @} */


		/** @{ */
		DfaStateMachine(DfaStateMachine&&) = default;
		DfaStateMachine& operator=(DfaStateMachine&&) = default;
		/** @} */


		/** @{ */
		DfaStateMachine(const DfaStateMachine&) = delete;
		DfaStateMachine& operator=(const DfaStateMachine&) = delete;
		/** @} */


		/** @{ */
		/** @brief Put a new state into the DFA with a copy of the specified "positions" set, if it doesn't already exist. */
		State PutState(const PositionSet& positions) {
			return PutStateImpl(positions);
		}
		
		/** @brief Put a new state into the DFA with the specified "positions" set, if it doesn't already exist. */
		State PutState(PositionSet&& positions) {
			return PutStateImpl(std::move(positions));
		}

		/** @brief Retrieve the complete list of states owned by the DFA. */
		const StateList& GetStates() const noexcept {
			return states;
		}

		/** @brief Get the number of states owned by the DFA. */
		std::size_t GetStatesCount() const noexcept {
			return states.size();
		}
		/** @} */
		

		/** @{ */
		/** @brief Add a new transition to the DFA. */
		void AddTransition(const State& from, const State& to, char label) {
			transitions.emplace_back(from, to, label);
		}

		/** @brief Get the list of DFA transitions. */
		const TransitionList& GetTransitions() const noexcept {
			return transitions;
		}
		
		/** @brief Get the number of transitions in the DFA. */
		std::size_t GetTransitionsCount() const noexcept {
			return GetTransitions().size();
		}
		/** @} */


		/** @{ */
		/** @brief Mark a state as an accepting state. */
		void AddAcceptingState(const State& state) {
			acceptingStates.push_back(state);
		}

		/** @brief Get the list of accepting states. */
		const StateList& GetAcceptingStates() const noexcept {
			return acceptingStates;
		}
		/** @} */


		/** @{ */
		/** @brief Mark a state as the starting state. */
		void SetStartState(const State& state) noexcept {
			startState = state;
		}

		/** @brief Get the starting state of the DFA. */
		const State& GetStartState() const noexcept {
			return startState;
		}
		/** @} */


		/** @{ */
		/** @brief Print out all the information about the DFA. */
		void Dump(std::ostream& out = std::cout) const;
		/** @} */

	private:
		/** @{ */
		template <typename PS>
		State PutStateImpl(PS&& positions) {
			// cache the set of positions for the state and give it a unique ID
			const auto stateId = stateIds.try_emplace(std::forward<PS>(positions), stateIds.size());
			// construct a State from the cached positions
			const auto state = State(*stateId.first);
			if(stateId.second) {
				states.push_back(state);
			}
			return state;
		}
		/** @} */

		TransitionList transitions;

		StateIdsMap stateIds;
		StateList states;

		StateList acceptingStates;
		State startState;
	};
}

#endif