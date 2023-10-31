#ifndef PARSEC_DFA_STATE_GEN_HEADER
#define PARSEC_DFA_STATE_GEN_HEADER

#include "../fg/Grammar.hpp"
#include "State.hpp"

namespace parsec::dfa {
	class StateGen {
	public:
		/** @{ */
		StateGen() = default;
		/** @} */


		/** @{ */
		StateGen(StateGen&&) = default;
		StateGen& operator=(StateGen&&) = default;
		/** @} */

		/** @{ */
		StateGen(const StateGen&) = delete;
		StateGen& operator=(const StateGen&) = delete;
		/** @} */


		/** @{ */
		StateList run(const fg::Grammar& grammar);
		/** @} */
	};
}

#endif