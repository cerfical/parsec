#ifndef PARSEC_ELR_PRINT_HEADER
#define PARSEC_ELR_PRINT_HEADER

#include <iostream>
#include <string_view>

namespace parsec::elr {
	
	void print(const class State& state, std::ostream& out = std::cout, std::string_view indent = "");

	void print(const class Automaton& elr, std::ostream& out = std::cout, std::string_view indent = "");
	
	void print(const class TransNetwork& transNet, std::ostream& out = std::cout, std::string_view indent = "");

	void print(const class ItemSet& itemSet, std::ostream& out = std::cout, std::string_view indent = "");


	std::ostream& operator<<(std::ostream& out, const class ReduceAction& reduce);
	
	std::ostream& operator<<(std::ostream& out, const class State& state);

	std::ostream& operator<<(std::ostream& out, const class StateTrans& trans);
	
	std::ostream& operator<<(std::ostream& out, const class StateItem& item);

}

#endif