#ifndef PARSEC_DFA_PRINT_HEADER
#define PARSEC_DFA_PRINT_HEADER

#include <iostream>
#include <string_view>

namespace parsec::dfa {

	void print(const class State& state, std::ostream& out = std::cout, std::string_view indent = "");

	void print(const class Automaton& dfa, std::ostream& out = std::cout, std::string_view indent = "");


	std::ostream& operator<<(std::ostream& out, const class State& state);

	std::ostream& operator<<(std::ostream& out, const class StateTrans& trans);

}

#endif