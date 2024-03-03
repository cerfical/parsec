#ifndef PARSEC_FSM_AUTOMATON_FACTORY_HEADER
#define PARSEC_FSM_AUTOMATON_FACTORY_HEADER

#include "../core/NonMovable.hpp"
#include "../fg/RegularGrammar.hpp"
#include "../fg/Grammar.hpp"

#include "Automaton.hpp"

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



		/**
		 * @brief Construct a DFA (Determinstic Finite Automaton) from its regular language.
		*/
		Automaton makeDfa(const fg::RegularGrammar& regLang);



		/**
		 * @brief Construct an SLR(1) automaton.
		*/
		Automaton makeSlr(const fg::Grammar& lang);



	private:
		AutomatonFactory() = default;
		~AutomatonFactory() = default;
	};

}

#endif