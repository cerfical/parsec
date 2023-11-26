#ifndef PARSEC_LR_STATE_HEADER
#define PARSEC_LR_STATE_HEADER

#include "Transition.hpp"
#include "StateReduce.hpp"

#include <concepts>
#include <ranges>
#include <vector>

namespace parsec::lr {
	/**
	 * @brief SLR state with all its shifts, goto table and reduce actions.
	 */
	class State {
	public:
		State(int id, const Symbol* inputSymbol) noexcept
			: m_inputSymbol(inputSymbol), m_id(id)
		{ }


		/** @{ */
		State(const State&) = default;
		State& operator=(const State&) = default;
		/** @} */
		
		/** @{ */
		State(State&&) = default;
		State& operator=(State&&) = default;
		/** @} */


		/** @{ */
		template <typename... Args>
			requires std::constructible_from<Transition, Args...>
		void addShift(Args&&... args) {
			m_shifts.emplace_back(std::forward<Args>(args)...);
		}

		std::ranges::view auto shifts() const {
			return m_shifts | std::views::transform(
				[](const auto& sh) { return &sh; }
			);
		}
		/** @} */


		/** @{ */
		template <typename... Args>
			requires std::constructible_from<Transition, Args...>
		void addGoto(Args&&... args) {
			m_gotos.emplace_back(std::forward<Args>(args)...);
		}

		std::ranges::view auto gotos() const {
			return m_gotos | std::views::transform(
				[](const auto& gt) { return &gt; }
			);
		}
		/** @} */


		/** @{ */
		template <typename... Args>
			requires std::constructible_from<StateReduce, Args...>
		void addReduce(Args&&... args) {
			m_reduces.emplace_back(std::forward<Args>(args)...);
		}

		std::ranges::view auto reduces() const {
			return m_reduces | std::views::transform(
				[](const auto& r) { return &r; }
			);
		}
		/** @} */


		/** @{ */
		int id() const noexcept {
			return m_id;
		}
		
		const Symbol* inputSymbol() const noexcept {
			return m_inputSymbol;
		}
		
		bool isReducing() const noexcept {
			return !m_reduces.empty();
		}

		bool isStart() const noexcept {
			return m_id == 0;
		}
		/** @} */


	private:
		std::vector<Transition> m_shifts;
		std::vector<Transition> m_gotos;
		std::vector<StateReduce> m_reduces;
		
		const Symbol* m_inputSymbol;
		int m_id;
	};
}

#endif