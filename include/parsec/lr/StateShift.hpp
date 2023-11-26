#ifndef PARSEC_LR_STATE_SHIFT_HEADER
#define PARSEC_LR_STATE_SHIFT_HEADER

#include "../core/Symbol.hpp"

namespace parsec::lr {
	class StateShift {
	public:
		/** @{ */
		StateShift(const Symbol* inputSymbol, int newState) noexcept
			: m_inputSymbol(inputSymbol), m_newState(newState)
		{ }
		/** @} */


		/** @{ */
		StateShift(const StateShift&) = default;
		StateShift& operator=(const StateShift&) = default;
		/** @} */

		
		/** @{ */
		const Symbol* inputSymbol() const noexcept {
			return m_inputSymbol;
		}
		
		int newState() const noexcept {
			return m_newState;
		}
		/** @} */


	private:
		const Symbol* m_inputSymbol;
		int m_newState;
	};
}

#endif