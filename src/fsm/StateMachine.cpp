#include "fsm/StateMachine.hpp"
#include <format>

namespace parsec::fsm {
	void print(const StateMachine& fsm, std::string_view indent, std::ostream& out) {
		out << indent << "State Machine" << (fsm ?
				std::format(", {} states{}:",
					fsm.states().size(),
					fsm.startState() ?
						std::format(" starting #{}", fsm.startState().id()) :
						""
				) :
				": Empty")
			<< '\n';

		for(const auto& state : fsm.states()) {
			print(state, std::string(indent) + '\t', out);
		}
	}


	const State& StateMachine::stateById(int state) const {
		const auto idToIndexIt = m_idToIndex.find(state);
		if(idToIndexIt == m_idToIndex.end()) {
			static State emptyState;
			return emptyState;
		}
		return stateByIndex(idToIndexIt->second);
 	}


	void StateMachine::markAcceptState(int state, const fg::Symbol& inputMatch) {
		const auto stateIndex = insertState(state);
		try {
			stateByIndex(stateIndex).setInputMatch(inputMatch);
		} catch(...) {
			eraseState(state);
			throw;
		}
	}


	void StateMachine::buildTransition(int from, int to, const fg::Symbol& label) {
		const auto fromIndex = insertState(from);
		try {
			insertState(to);
			try {
				stateByIndex(fromIndex).addTransitionTo(to, label);
			} catch(...) {
				eraseState(to);
				throw;
			}
		} catch(...) {
			eraseState(from);
			throw;
		}
	}


	std::size_t StateMachine::insertState(int state) {
		const auto [idToIndexIt, wasInserted] = m_idToIndex.try_emplace(state, m_states.size());
		if(wasInserted) {
			try {
				m_states.emplace_back(state);
			} catch(...) {
				m_idToIndex.erase(idToIndexIt);
				throw;
			}
		}
		return idToIndexIt->second;
	}


	void StateMachine::eraseState(int state) noexcept {
		const auto idToIntexIt = m_idToIndex.find(state);
		if(idToIntexIt != m_idToIndex.end()) {
			const auto stateIt = std::next(m_states.begin(), idToIntexIt->second);
			m_idToIndex.erase(idToIntexIt);
			m_states.erase(stateIt);
		}
	}
}