#ifndef PARSEC_LR_TRANSITION_HEADER
#define PARSEC_LR_TRANSITION_HEADER

#include "../core/Symbol.hpp"

namespace parsec::lr {
	/**
	 * @brief Defines a state transition on a grammar symbol.
	 */
	class Transition {
	public:
		Transition(const Symbol* inputSymbol, int newState) noexcept
			: m_inputSymbol(inputSymbol), m_newState(newState)
		{ }


		/** @{ */
		Transition(const Transition&) = default;
		Transition& operator=(const Transition&) = default;
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