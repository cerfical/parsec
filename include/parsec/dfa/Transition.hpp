#ifndef PARSEC_DFA_TRANSITION_HEADER
#define PARSEC_DFA_TRANSITION_HEADER

#include <vector>

namespace parsec::dfa {
	class Transition {
	public:
		/** @{ */
		Transition(int state, char label) noexcept
			: m_state(state), m_label(label)
		{ }
		/** @} */


		/** @{ */
		Transition(const Transition&) = default;
		Transition& operator=(const Transition&) = default;
		/** @} */


		/** @{ */
		int state() const noexcept {
			return m_state;
		}

		char label() const noexcept {
			return m_label;
		}
		/** @} */


	private:
		/** @{ */
		int m_state;
		char m_label;
		/** @} */
	};


	using TransitionList = std::vector<Transition>;
}

#endif