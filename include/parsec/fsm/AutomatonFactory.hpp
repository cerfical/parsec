#ifndef PARSEC_FSM_AUTOMATON_FACTORY_HEADER
#define PARSEC_FSM_AUTOMATON_FACTORY_HEADER

#include "../core/NonMovable.hpp"

namespace parsec::fsm {

	/**
	 * @brief Constructs more elaborate automata from simple grammar descriptions of the languages they are supposed to recognize.
	*/
	class AutomatonFactory : private NonMovable {
	public:

		/**
		 * @brief Get access to the default factory.
		*/
		static AutomatonFactory& get();

	private:
		AutomatonFactory() = default;
		~AutomatonFactory() = default;
	};

}

#endif