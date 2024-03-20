#include "fsm/State.hpp"

namespace parsec::fsm {
	void print(const State& state, std::ostream& out, std::string_view indent) {
		out << "State #" << state.id() << ":\n";
		for(const auto& trans : state.transitions()) {
			out << '\t' << trans.label() << " => " << trans.target() << '\n';
		}
	}
}