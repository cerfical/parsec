#ifndef PARSEC_LR_STATE_GOTO_HEADER
#define PARSEC_LR_STATE_GOTO_HEADER

#include "../Symbol.hpp"

namespace parsec::lr {
	class StateGoto {
	public:
		/** @{ */
		StateGoto(const Symbol* inputSymbol, int newState) noexcept
			: m_inputSymbol(inputSymbol), m_newState(newState)
		{ }
		/** @} */


		/** @{ */
		StateGoto(const StateGoto&) = default;
		StateGoto& operator=(const StateGoto&) = default;
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