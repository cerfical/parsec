#ifndef PARSEC_ELR_TRANS_NETWORK_HEADER
#define PARSEC_ELR_TRANS_NETWORK_HEADER

#include "../core/NonCopyable.hpp"
#include "../fg/SymbolGrammar.hpp"
#include "../dfa/Automaton.hpp"

#include <unordered_map>
#include <span>
#include <vector>

namespace parsec::elr {

	class TransNetwork : private NonCopyable {
	public:

		TransNetwork() = default;

		explicit TransNetwork(const fg::SymbolGrammar& grammar);

		
		const dfa::State& stateById(int state) const;
		
		const dfa::State& startState(const fg::Symbol& symbol) const;

		std::span<const dfa::State> states() const {
			return m_states;
		}


		explicit operator bool() const {
			return !isEmpty();
		}

		bool isEmpty() const {
			return m_states.empty();
		}


	private:
		void insertMachine(const fg::Symbol& name, const dfa::Automaton& dfa);

		std::vector<dfa::State> m_states;
		std::unordered_map<fg::Symbol, int> m_symbolToStartState;
	};

}

#endif