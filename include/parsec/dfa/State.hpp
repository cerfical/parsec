#ifndef PARSEC_DFA_STATE_HEADER
#define PARSEC_DFA_STATE_HEADER

#include "StateTrans.hpp"

#include <vector>
#include <span>

namespace parsec::dfa {

	class State {
	public:

		explicit State(int id)
			: m_id(id) {}


		std::span<const StateTrans> transitions() const {
			return m_transitions;
		}

		void addTransition(int target, const fg::Symbol& label) {
			m_transitions.emplace_back(target, label);
		}
		

		bool isAcceptState() const {
			return !acceptSymbol().isEmpty();
		}

		const fg::Symbol& acceptSymbol() const {
			return m_acceptSymbol;
		}

		void setAcceptSymbol(const fg::Symbol& symbol) {
			m_acceptSymbol = symbol;
		}


		bool isStartState() const {
			return m_startState;
		}

		void setStartState(bool startState) {
			m_startState = startState;
		}


		int id() const {
			return m_id;
		}


	private:
		std::vector<StateTrans> m_transitions;
		fg::Symbol m_acceptSymbol;
		int m_id = {};
		bool m_startState = false;
	};

}

#endif