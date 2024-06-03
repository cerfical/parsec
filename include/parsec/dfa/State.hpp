#pragma once

#include "../core/Symbol.hpp"

#include <vector>

namespace parsec::dfa {

	struct StateTrans {
		int target = {};
		Symbol label;
	};


	class State {
	public:

		State() noexcept = default;

		explicit State(int id) noexcept
			: m_id(id) {}


		const std::vector<StateTrans>& transitions() const noexcept {
			return m_transitions;
		}


		void addTransition(int target, const Symbol& label) {
			m_transitions.emplace_back(target, label);
		}
		

		bool isMatchState() const noexcept {
			return !matchedRule().isEmpty();
		}


		const Symbol& matchedRule() const noexcept {
			return m_matchedRule;
		}


		void setMatchedRule(const Symbol& rule) noexcept {
			m_matchedRule = rule;
		}


		bool isStartState() const noexcept {
			return m_startState;
		}


		void setStartState(bool startState) noexcept {
			m_startState = startState;
		}


		explicit operator bool() const noexcept {
			return !isEmpty();
		}


		bool isEmpty() const noexcept {
			return m_transitions.empty() && !isStartState() && !isMatchState();
		}


		int id() const noexcept {
			return m_id;
		}


	private:
		std::vector<StateTrans> m_transitions;
		Symbol m_matchedRule;

		int m_id = {};
		bool m_startState = false;
	};

}
