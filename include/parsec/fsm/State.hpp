#ifndef PARSEC_FSM_STATE_HEADER
#define PARSEC_FSM_STATE_HEADER

#include "../core/NonCopyable.hpp"
#include "StateTrans.hpp"

#include <vector>
#include <iostream>
#include <span>

namespace parsec::fsm {

	class State : private NonCopyable {
	public:

		friend bool operator==(const State& lhs, const State& rhs) {
			return lhs.id() == rhs.id();
		}


		State() = default;

		explicit State(int id)
			: m_id(id) {}


		void addTransitionTo(int state, const fg::Symbol& label) {
			m_transitions.emplace_back(state, label);
		}

		std::span<const StateTrans> transitions() const {
			return m_transitions;
		}


		void setInputMatch(const fg::Symbol& match) {
			m_inputMatch = match;
		}

		const fg::Symbol& inputMatch() const {
			return m_inputMatch;
		}


		int id() const {
			return m_id;
		}


		explicit operator bool() const {
			return !isEmpty();
		}

		bool isEmpty() const {
			return m_transitions.empty() && m_inputMatch.isEmpty();
		}


	private:
		fg::Symbol m_inputMatch;
		std::vector<StateTrans> m_transitions;
		int m_id = {};
	};

	void print(const State& state, std::string_view indent = "", std::ostream& out = std::cout);

}

#endif