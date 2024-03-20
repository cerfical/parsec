#include "fsm/StateMachine.hpp"

#include <algorithm>
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


	std::span<const State> StateMachine::states() const {
		if(!m_statesSorted) {
			std::ranges::sort(m_states, [](const auto& lhs, const auto& rhs) {
				return lhs.id() < rhs.id();
			});

			m_idToIndexCache.clear();
			m_statesSorted = true;
		}
		return m_states;
	}


	const State& StateMachine::stateById(int state) const {
		updateCache();
		// no need to cancel the cache update activity

		const auto idToIndexIt = m_idToIndexCache.find(state);
		if(idToIndexIt == m_idToIndexCache.end()) {
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
		updateCache();
		// no need to cancel the cache update activity

		const auto [idToIndexIt, wasInserted] = m_idToIndexCache.try_emplace(state, m_states.size());
		if(wasInserted) {
			try {
				m_states.emplace_back(state);
			} catch(...) {
				m_idToIndexCache.erase(idToIndexIt);
				throw;
			}
			m_statesSorted = false;
		}

		return idToIndexIt->second;
	}


	void StateMachine::eraseState(int state) noexcept {
		// before calling eraseState(), the cache must be valid
		const auto idToIndexIt = m_idToIndexCache.find(state);
		if(idToIndexIt != m_idToIndexCache.end()) {
			const auto stateIt = std::next(m_states.begin(), idToIndexIt->second);
			m_idToIndexCache.erase(idToIndexIt);
			m_states.erase(stateIt);
		}
		// state removal does not change the sorted order of the states() list
	}


	void StateMachine::updateCache() const {
		if(m_idToIndexCache.empty()) {
			for(std::size_t i = 0; i < m_states.size(); i++) {
				m_idToIndexCache[stateByIndex(i).id()] = i;
			}
		}
	}
}