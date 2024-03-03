#include "fsm/AutomatonBuilder.hpp"
#include <algorithm>

namespace parsec::fsm {
	auto AutomatonBuilder::insertState(int state) {
		const auto [stateIt, wasInserted] = m_states.try_emplace(state);
		return std::pair(stateIt, [this, wasInserted, stateIt]() noexcept {
			if(wasInserted) {
				m_states.erase(stateIt);
			}
		});
	}


	AutomatonBuilder& AutomatonBuilder::transition(std::string inSymbol, int src, int dest) {
		const auto [srcIt, abortSrcInsert] = insertState(src);
		auto& srcData = srcIt->second;

		try {
			srcData.trans.emplace_back(std::move(inSymbol), src, dest);
		} catch(...) {
			abortSrcInsert();
			throw;
		}

		try {
			insertState(dest);
		} catch(...) {
			srcData.trans.pop_back();
			abortSrcInsert();
			throw;
		}

		m_totalTrans++;
		return *this;
	}


	AutomatonBuilder& AutomatonBuilder::acceptState(int state, std::string outSymbol) {
		const auto [stateIt, abortStateInsert] = insertState(state);
		auto& stateData = stateIt->second;

		try {
			stateData.outSymbol = std::move(outSymbol);
		} catch(...) {
			abortStateInsert();
			throw;
		}

		return *this;
	}


	AutomatonBuilder& AutomatonBuilder::startState(int state) {
		insertState(state);
		m_startState.emplace(state);
		return *this;
	}


	Automaton AutomatonBuilder::build() {
		const auto byInSymbol = [](const auto& lhs, const auto& rhs) { return lhs.inSymbol < rhs.inSymbol; };
		auto impl = Automaton::Impl(m_states.size(), m_totalTrans);

		for(const auto& [id, stateData] : m_states) {
			impl.trans.insert(impl.trans.end(), stateData.trans.begin(), stateData.trans.end());

			const auto transBegin = (impl.trans.data() + impl.trans.size()) - stateData.trans.size();
			const auto stateTrans = std::span(transBegin, stateData.trans.size());

			std::ranges::sort(stateTrans, byInSymbol);

			impl.stateIndices[id] = impl.states.size();
			impl.states.emplace_back(
				stateTrans,
				stateData.outSymbol.value_or(""),
				id
			);
		}

		
		impl.startState = std::exchange(m_startState, {});
		m_states.clear();
		m_totalTrans = 0;

		return Automaton(std::move(impl));
	}
}