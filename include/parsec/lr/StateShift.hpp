#ifndef PARSEC_LR_STATE_SHIFT_HEADER
#define PARSEC_LR_STATE_SHIFT_HEADER

#include "../fg/Symbol.hpp"
#include <vector>

namespace parsec::lr {
	class StateShift {
	public:
		/** @{ */
		StateShift(const fg::Symbol* symbol, int state) noexcept
			: m_symbol(symbol), m_state(state)
		{ }
		/** @} */


		/** @{ */
		StateShift(const StateShift&) = default;
		StateShift& operator=(const StateShift&) = default;
		/** @} */

		
		/** @{ */
		const fg::Symbol* inputSymbol() const noexcept {
			return m_symbol;
		}
		
		int newState() const noexcept {
			return m_state;
		}
		/** @} */


	private:
		const fg::Symbol* m_symbol;
		int m_state;
	};

	using ShiftList = std::vector<StateShift>;
}

#endif