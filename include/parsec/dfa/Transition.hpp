#ifndef PARSEC_DFA_TRANSITION_HEADER
#define PARSEC_DFA_TRANSITION_HEADER

namespace parsec::dfa {
	class Transition {
	public:
		/** @{ */
		Transition(int newState, char inputChar) noexcept
			: m_newState(newState), m_inputChar(inputChar)
		{ }
		/** @} */


		/** @{ */
		Transition(const Transition&) = default;
		Transition& operator=(const Transition&) = default;
		/** @} */


		/** @{ */
		char inputChar() const noexcept {
			return m_inputChar;
		}
		
		int newState() const noexcept {
			return m_newState;
		}
		/** @} */

	private:
		int m_newState;
		char m_inputChar;
	};
}

#endif