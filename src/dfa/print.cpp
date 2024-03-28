#include "dfa.hpp"

namespace parsec::dfa {
	void print(const State& state, std::ostream& out, std::string_view indent) {
		out << indent;
		if(state.isStartState()) {
			out << "=> ";
		}
		out << state;
		if(state.isMatchState()) {
			out << " => " << state.matchedRule();
		}
		out << '\n';

		for(const auto& trans : state.transitions()) {
			out << indent << "  " << trans << '\n';
		}
	}


	void print(const Automaton& dfa, std::ostream& out, std::string_view indent) {
		for(const auto& state : dfa.states()) {
			print(state, out, indent);
		}
	}


	std::ostream& operator<<(std::ostream& out, const StateTrans& trans) {
		return out << trans.label() << " => " << State(trans.target());
	}


	std::ostream& operator<<(std::ostream& out, const State& state) {
		return out << "( " << state.id() << " )";
	}
}