#ifndef PARSEC_DFA_STATE_HEADER
#define PARSEC_DFA_STATE_HEADER

#include "Match.hpp"
#include "Transition.hpp"

#include <vector>

namespace parsec::dfa {
	class State {
	public:
		/** @{ */
		explicit State(int id = 0) noexcept
			: m_id(id)
		{ }
		/** @} */


		/** @{ */
		State(const State&) = default;
		State& operator=(const State&) = default;
		/** @} */

		/** @{ */
		State(State&&) = default;
		State& operator=(State&&) = default;
		/** @} */


		/** @{ */
		const TransitionList& transitions() const noexcept {
			return m_transitions;
		}

		template <typename... Args>
			requires std::constructible_from<Transition, Args...>
		void addTransition(Args&&... args) {
			m_transitions.emplace_back(std::forward<Args>(args)...);
		}
		/** @} */


		/** @{ */
		const MatchList& matches() const noexcept {
			return m_matches;
		}

		template <typename... Args>
			requires std::constructible_from<Match, Args...>
		void addMatch(Args&&... args) {
			m_matches.emplace_back(std::forward<Args>(args)...);
		}
		/** @} */


		/** @{ */
		bool isAccepting() const noexcept {
			return !m_matches.empty();
		}

		bool isStart() const noexcept {
			return m_id == 0;
		}

		int id() const noexcept {
			return m_id;
		}
		/** @} */


	private:
		TransitionList m_transitions;
		MatchList m_matches;
		int m_id;
	};

	using StateList = std::vector<State>;
}

#endif