#ifndef PARSEC_LR_SHIFT_ACTION_HEADER
#define PARSEC_LR_SHIFT_ACTION_HEADER

#include "../fg/Symbol.hpp"
#include <vector>

namespace parsec::lr {
	class ShiftAction {
	public:
		/** @{ */
		ShiftAction(const fg::Symbol* symbol, int state) noexcept
			: m_symbol(symbol), m_state(state)
		{ }
		/** @} */


		/** @{ */
		ShiftAction(const ShiftAction&) = default;
		ShiftAction& operator=(const ShiftAction&) = default;
		/** @} */

		
		/** @{ */
		const fg::Symbol* symbol() const noexcept {
			return m_symbol;
		}
		
		int state() const noexcept {
			return m_state;
		}
		/** @} */


	private:
		/** @{ */
		const fg::Symbol* m_symbol;
		int m_state;
		/** @} */
	};


	using ShiftActionList = std::vector<ShiftAction>;
}

#endif