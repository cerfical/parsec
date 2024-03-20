#include "fsm/State.hpp"
#include <format>

namespace parsec::fsm {
	void print(const State& state, std::string_view indent, std::ostream& out) {
		out << indent << std::format("State #{}{}{}", state.id(),
			state.inputMatch() ? std::format("({})", state.inputMatch().value()) : "",
			state.transitions().empty() ? "" : ":"
		) << '\n';

		for(const auto& trans : state.transitions()) {
			out << indent << '\t' << trans.label() << " => " << trans.target() << '\n';
		}
	}
}