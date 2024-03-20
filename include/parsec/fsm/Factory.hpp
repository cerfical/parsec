#ifndef PARSEC_FSM_FACTORY_HEADER
#define PARSEC_FSM_FACTORY_HEADER

#include "../core/NonMovable.hpp"
#include "../core/NonCopyable.hpp"
#include "../fg/SymbolGrammar.hpp"

#include "StateMachine.hpp"

namespace parsec::fsm {

	class Factory : private NonCopyable, private NonMovable {
	public:

		static Factory* get();


		StateMachine makeDfa(const fg::SymbolGrammar& regLang);

		StateMachine makeSlr(const fg::SymbolGrammar& lang);


	private:
		Factory() = default;
		~Factory() = default;
	};

}

#endif