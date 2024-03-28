#ifndef PARSEC_DFA_STATE_HEADER
#define PARSEC_DFA_STATE_HEADER

#include "StateTrans.hpp"

#include <vector>
#include <span>

namespace parsec::dfa {

	class State {
	public:

		State() = default;

		explicit State(int id)
			: m_id(id) {}


		std::span<const StateTrans> transitions() const {
			return m_transitions;
		}

		void addTransition(int target, const fg::Symbol& label) {
			m_transitions.emplace_back(target, label);
		}
		

		bool isMatchState() const {
			return !matchedRule().isEmpty();
		}

		const fg::Symbol& matchedRule() const {
			return m_matchedRule;
		}

		void setMatchedRule(const fg::Symbol& rule) {
			m_matchedRule = rule;
		}


		bool isStartState() const {
			return m_startState;
		}

		void setStartState(bool startState) {
			m_startState = startState;
		}


		explicit operator bool() const {
			return !isEmpty();
		}

		bool isEmpty() const {
			return m_transitions.empty() && !isStartState() && !isMatchState();
		}

		int id() const {
			return m_id;
		}


	private:
		std::vector<StateTrans> m_transitions;
		fg::Symbol m_matchedRule;
		int m_id = {};
		bool m_startState = false;
	};

}

#endif