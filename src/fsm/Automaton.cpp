#include "fsm/Automaton.hpp"

namespace parsec::fsm {
	const State* Automaton::stateById(int state) const {
		const auto stateIt = m_impl.stateIndices.find(state);
		if(stateIt != m_impl.stateIndices.end()) {
			return &stateByIndex(stateIt->second);
		}
		return nullptr;
	}
}