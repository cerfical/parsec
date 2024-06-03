#pragma once

#include "../core/Symbol.hpp"

#include <vector>

namespace parsec::elr {

	struct StateTrans {
		int target = {};
		Symbol label;
	};


	struct ReduceAction {
		Symbol reduceRule;
		int backLink = {};
	};


	class State {
	public:

		State() noexcept = default;

		explicit State(int id) noexcept
			: m_id(id) {}


		const std::vector<StateTrans>& shifts() const noexcept {
			return m_shifts;
		}


		void addShift(int target, const Symbol& label) {
			m_shifts.emplace_back(target, label);
		}


		const std::vector<StateTrans>& gotos() const noexcept {
			return m_gotos;
		}


		void addGoto(int target, const Symbol& label) {
			m_gotos.emplace_back(target, label);
		}


		const std::vector<int>& backLinks() const noexcept {
			return m_backLinks;
		}


		void addBackLink(int backLink) {
			m_backLinks.emplace_back(backLink);
		}
		

		bool isReduceState() const noexcept {
			return !reduction().reduceRule.isEmpty();
		}


		const ReduceAction& reduction() const noexcept {
			return m_reduction;
		}


		void setReduction(const Symbol& reduceRule, int backLink) noexcept {
			m_reduction = ReduceAction(reduceRule, backLink);
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
			return m_shifts.empty() && m_gotos.empty() && !isStartState() && !isReduceState();
		}


		int id() const noexcept {
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
