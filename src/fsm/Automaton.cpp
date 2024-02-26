#include "fsm/Automaton.hpp"

#include <algorithm>
#include <stdexcept>
#include <iterator>

namespace parsec::fsm {
	auto Automaton::Builder::insertState(int state) {
		const auto [stateIt, wasInserted] = m_states.try_emplace(state);
		return std::pair(stateIt, [this, wasInserted, stateIt]() noexcept {
			if(wasInserted) {
				m_states.erase(stateIt);
			}
		});
	}


	Automaton::Builder& Automaton::Builder::addTransition(Symbol inSymbol, int src, int dest) {
		// cache the source state of the transition and the transition itself
		const auto [srcIt, abortSrcInsert] = insertState(src);
		auto& srcData = srcIt->second;

		try {
			srcData.trans.emplace_back(inSymbol, src, dest);
		} catch(...) {
			abortSrcInsert();
			throw;
		}

		try {
			// cache the destination state and make sure that it has a correct input symbol set up
			const auto [destIt, abortDestInsert] = insertState(dest);
			auto& destData = destIt->second;

			if(!destData.inSymbol.has_value()) {
				try {
					destData.inSymbol = inSymbol;
				} catch(...) {
					abortDestInsert();
					throw;
				}
			} else if(destData.inSymbol != inSymbol) {
				abortDestInsert();
				throw std::runtime_error(
					"an attempt was made to create an incosistent state transition"
				);
			}
		} catch(...) {
			srcData.trans.pop_back();
			abortSrcInsert();
			throw;
		}

		m_totalTrans++;
		return *this;
	}


	Automaton::Builder& Automaton::Builder::setAcceptState(int state, Symbol outSymbol) {
		const auto [stateIt, abortStateInsert] = insertState(state);
		auto& stateData = stateIt->second;

		try {
			stateData.outSymbol = outSymbol;
		} catch(...) {
			abortStateInsert();
			throw;
		}

		return *this;
	}


	Automaton::Builder& Automaton::Builder::setStartState(int state) {
		insertState(state);
		m_startState.emplace(state);
		return *this;
	}


	Automaton Automaton::Builder::finishBuild() {
		auto impl = Impl(m_states.size(), m_totalTrans);

		for(const auto& [id, data] : m_states) {
			std::ranges::copy(data.trans, std::back_inserter(impl.trans));

			const auto transBegin = (impl.trans.data() + impl.trans.size()) - data.trans.size();
			impl.stateIndices[id] = impl.states.size();
			impl.states.emplace_back(
				std::span(transBegin, data.trans.size()),
				data.inSymbol.value_or(Symbol()),
				data.outSymbol.value_or(Symbol()),
				id
			);
		}

		impl.startState = m_startState;
		m_states.clear();
		m_totalTrans = 0;

		return Automaton(std::move(impl));
	}


	const State* Automaton::stateById(int state) const {
		const auto stateIt = m_impl.stateIndices.find(state);
		if(stateIt != m_impl.stateIndices.end()) {
			return stateByIndex(stateIt->second);
		}
		return nullptr;
	}


	Automaton::Builder Automaton::startBuild() {
		return Builder();
	}
}