#ifndef PARSEC_LR_STATE_GOTO_HEADER
#define PARSEC_LR_STATE_GOTO_HEADER

#include "../fg/Symbol.hpp"
#include <vector>

namespace parsec::lr {
	class StateGoto {
	public:
		/** @{ */
		StateGoto(const fg::Symbol* symbol, int state) noexcept
			: m_symbol(symbol), m_state(state)
		{ }
		/** @} */


		/** @{ */
		StateGoto(const StateGoto&) = default;
		StateGoto& operator=(const StateGoto&) = default;
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

	using GotoList = std::vector<StateGoto>;
}

#endif