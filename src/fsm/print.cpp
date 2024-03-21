#include "fsm.hpp"
#include <format>

namespace parsec::fsm {
	namespace {
		void printState(const State& state, bool isStart, std::ostream& out, std::string_view indent) {
			out << indent <<
				std::format("{}{}{}{}",
					isStart ? "=> " : "",
					state.id(),
					state.inputMatch() ? std::format(" => {}", state.inputMatch().value()) : "",
					state.transitions().empty() ? "" : ":"
				) << '\n';

			for(const auto& trans : state.transitions()) {
				out << indent << '\t' << trans << '\n';
			}
		}
	}


	void print(const State& state, std::ostream& out, std::string_view indent) {
		printState(state, false, out, indent);
	}


	void print(const StateMachine& fsm, std::ostream& out, std::string_view indent) {
		for(const auto& state : fsm.states()) {
			printState(state, state == fsm.startState(), out, indent);
		}
	}


	std::ostream& operator<<(std::ostream& out, const StateTrans& trans) {
		return out << trans.label() << " => " << trans.target();
	}
}