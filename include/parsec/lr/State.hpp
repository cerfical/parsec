#ifndef PARSEC_LR_STATE_HEADER
#define PARSEC_LR_STATE_HEADER

#include "StateShift.hpp"
#include "StateGoto.hpp"
#include "StateReduce.hpp"

#include <concepts>
#include <vector>

namespace parsec::lr {
	class State {
	public:
		/** @{ */
		State() = default;

		State(int id, const fg::Symbol* symbol) noexcept
			: m_symbol(symbol), m_id(id)
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
			requires std::constructible_from<StateShift, Args...>
		void addShift(Args&&... args) {
			m_shifts.emplace_back(std::forward<Args>(args)...);
		}

		const ShiftList& shifts() const noexcept {
			return m_shifts;
		}
		
		bool anyShifts() const noexcept {
			return !m_shifts.empty();
		}
		/** @} */


		/** @{ */
		template <typename... Args>
			requires std::constructible_from<StateGoto, Args...>
		void addGoto(Args&&... args) {
			m_gotos.emplace_back(std::forward<Args>(args)...);
		}

		const GotoList& gotos() const noexcept {
			return m_gotos;
		}

		bool anyGotos() const noexcept {
			return !m_gotos.empty();
		}
		/** @} */


		/** @{ */
		template <typename... Args>
			requires std::constructible_from<StateReduce, Args...>
		void addReduce(Args&&... args) {
			m_reduces.emplace_back(std::forward<Args>(args)...);
		}

		const ReduceList& reduces() const noexcept {
			return m_reduces;
		}
		
		bool isReducing() const noexcept {
			return !m_reduces.empty();
		}
		/** @} */


		/** @{ */
		const fg::Symbol* symbol() const noexcept {
			return m_symbol;
		}

		int id() const noexcept {
			return m_id;
		}
		
		bool isStart() const noexcept {
			return m_id == 0;
		}
		/** @} */


	private:
		ShiftList m_shifts;
		GotoList m_gotos;
		ReduceList m_reduces;
		
		const fg::Symbol* m_symbol = {};
		int m_id = 0;
	};

	using StateList = std::vector<State>;
}

#endif