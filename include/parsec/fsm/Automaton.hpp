#ifndef PARSEC_FSM_AUTOMATON_HEADER
#define PARSEC_FSM_AUTOMATON_HEADER

#include "../core/NonCopyable.hpp"

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <span>

namespace parsec::fsm {

	/**
	 * @brief Defines a jump of an Automaton to a new State upon encountering a specific input symbol.
	*/
	struct StateTrans {
		
		friend bool operator==(const StateTrans& lhs, const StateTrans& rhs) {
			return std::tie(lhs.src, lhs.dest) == std::tie(rhs.src, rhs.dest);
		}


		/** @brief An input symbol that triggers the state transition. */
		std::string inSymbol;

		/** @brief The originating state of the transition. */
		int src = {};
		
		/** @brief A new state of the Automaton after the transition. */
		int dest = {};
	};



	/**
	 * @brief Defines one of possible operational states of an Automaton.
	*/
	struct State {

		friend bool operator==(const State& lhs, const State& rhs) {
			return lhs.id == rhs.id;
		}


		/** @brief List of transitions originating from the state. */
		std::span<const StateTrans> transitions;
		
		/** @brief A symbol that the state can produce, if the Automaton can take no more transitions. */
		std::string outSymbol;
		
		/** @brief A unique integer identifier for the state. */
		int id = {};
	};



	/**
	 * @brief Implementation of the finite state machine formalism.
	*/
	class Automaton : private NonCopyable {
	public:

		Automaton() = default;



		/**
		 * @brief List of all transitions of the automaton.
		*/
		std::span<const StateTrans> transitions() const {
			return m_impl.trans;
		}



		/**
		 * @brief List of all states of the automaton.
		*/
		std::span<const State> states() const {
			return m_impl.states;
		}



		/**
		 * @brief Find a State in the automaton with the specified identifier, if there is one.
		*/
		const State* stateById(int state) const;



		/**
		 * @brief The initial operating state of the automaton, if there is one.
		*/
		const State* startState() const {
			if(m_impl.startState.has_value()) {
				return stateById(m_impl.startState.value());
			}
			return nullptr;
		}



	private:
		friend class AutomatonBuilder;


		const State& stateByIndex(std::size_t i) const {
			return m_impl.states[i];
		}


		struct Impl {
			Impl() = default;

			Impl(std::size_t stateCount, std::size_t transCount) {
				states.reserve(stateCount);
				trans.reserve(transCount);
				stateIndices.reserve(stateCount);
			}

			std::vector<State> states;
			std::unordered_map<int, std::size_t> stateIndices;
			std::vector<StateTrans> trans;
			std::optional<int> startState;
		};


		explicit Automaton(Impl&& impl)
			: m_impl(std::move(impl))
		{ }

		Impl m_impl;
	};

}

#endif