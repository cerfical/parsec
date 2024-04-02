#ifndef PARSEC_ELR_STATE_HEADER
#define PARSEC_ELR_STATE_HEADER

#include "StateTrans.hpp"
#include "ReduceAction.hpp"

#include <vector>
#include <span>

namespace parsec::elr {

	class State {
	public:

		State() = default;

		explicit State(int id)
			: m_id(id) {}


		std::span<const StateTrans> shifts() const {
			return m_shifts;
		}

		void addShift(int target, const Symbol& label) {
			m_shifts.emplace_back(target, label);
		}


		std::span<const StateTrans> gotos() const {
			return m_gotos;
		}

		void addGoto(int target, const Symbol& label) {
			m_gotos.emplace_back(target, label);
		}


		std::span<const int> backLinks() const {
			return m_backLinks;
		}

		void addBackLink(int backLink) {
			m_backLinks.emplace_back(backLink);
		}
		

		bool isReduceState() const {
			return !reduction().reduceRule().isEmpty();
		}

		const ReduceAction& reduction() const {
			return m_reduction;
		}

		void setReduction(const Symbol& reduceRule, int backLink) {
			m_reduction = ReduceAction(reduceRule, backLink);
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
			return m_shifts.empty() && m_gotos.empty() && !isStartState() && !isReduceState();
		}

		int id() const {
			return m_id;
		}


	private:
		std::vector<StateTrans> m_shifts;
		std::vector<StateTrans> m_gotos;
		
		std::vector<int> m_backLinks;
		ReduceAction m_reduction;

		int m_id = {};
		bool m_startState = false;
	};

}

#endif