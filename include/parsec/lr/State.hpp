#ifndef PARSEC_LR_STATE_HEADER
#define PARSEC_LR_STATE_HEADER

#include "ShiftAction.hpp"
#include "ReduceAction.hpp"

#include <concepts>
#include <vector>

namespace parsec::lr {
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
		template <typename... Args>
			requires std::constructible_from<ShiftAction, Args...>
		void addShift(Args&&... args) {
			m_shifts.emplace_back(std::forward<Args>(args)...);
		}

		const ShiftActionList& shifts() const noexcept {
			return m_shifts;
		}
		/** @} */


		/** @{ */
		template <typename... Args>
			requires std::constructible_from<ReduceAction, Args...>
		void addReduction(Args&&... args) {
			m_reductions.emplace_back(std::forward<Args>(args)...);
		}

		const ReduceActionList& reductions() const noexcept {
			return m_reductions;
		}
		/** @} */


		/** @{ */
		int id() const noexcept {
			return m_id;
		}
		/** @} */


	private:
		/** @{ */
		ShiftActionList m_shifts;
		ReduceActionList m_reductions;
		int m_id;
		/** @} */
	};


	using StateList = std::vector<State>;
}

#endif