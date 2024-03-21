#ifndef PARSEC_FSM_PRINT_HEADER
#define PARSEC_FSM_PRINT_HEADER

#include <iostream>
#include <string_view>

namespace parsec::fsm {

	void print(const class State& state, std::ostream& out = std::cout, std::string_view indent = "");
	
	void print(const class StateMachine& fsm, std::ostream& out = std::cout, std::string_view indent = "");
	
	std::ostream& operator<<(std::ostream& out, const class StateTrans& trans);

}

#endif