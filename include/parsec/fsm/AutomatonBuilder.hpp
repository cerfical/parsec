#ifndef PARSEC_FSM_AUTOMATON_BUILDER_HEADER
#define PARSEC_FSM_AUTOMATON_BUILDER_HEADER

#include "Automaton.hpp"

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

namespace parsec::fsm {

	/**
	 * @brief Serves as an interface to build an automaton of the desired configuration.
	*/
	class AutomatonBuilder : private NonCopyable {
	public:

		/**
		 * @brief Create a StateTrans transition in the automaton.
		*/
		AutomatonBuilder& transition(std::string inSymbol, int src, int dest);



		/**
		 * @brief Mark a state as accepting.
		*/
		AutomatonBuilder& acceptState(int state, std::string outSymbol);



		/**
		 * @brief Set the initial state of the automaton.
		*/
		AutomatonBuilder& startState(int state);



		/**
		 * @brief Create an Automaton with the requested configuration.
		*/
		Automaton build();



	private:
		auto insertState(int);


		struct StateData {
			std::vector<StateTrans> trans;
			std::optional<std::string> outSymbol;
		};

		std::unordered_map<int, StateData> m_states;
		std::optional<int> m_startState;
		std::size_t m_totalTrans = 0;
	};

}

#endif