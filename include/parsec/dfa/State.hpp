#ifndef PARSEC_DFA_STATE_HEADER
#define PARSEC_DFA_STATE_HEADER

#include "../Pattern.hpp"
#include "Transition.hpp"

#include <concepts>
#include <vector>
#include <ranges>

namespace parsec::dfa {
	class State {
	public:
		/** @{ */
		explicit State(int id) noexcept
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
		std::ranges::view auto transitions() const {
			return m_transitions | std::views::transform(
				[](const auto& t) { return &t; }
			);
		}

		template <typename... Args>
			requires std::constructible_from<Transition, Args...>
		void addTransition(Args&&... args) {
			m_transitions.emplace_back(std::forward<Args>(args)...);
		}
		/** @} */


		/** @{ */
		std::ranges::view auto matches() const {
			return std::ranges::ref_view(m_matches);
		}

		void addMatch(const Pattern* match) {
			m_matches.push_back(match);
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
		std::vector<Transition> m_transitions;
		std::vector<const Pattern*> m_matches;

		int m_id;
	};
}

#endif