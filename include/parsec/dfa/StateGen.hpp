#ifndef PARSEC_DFA_STATE_GEN_HEADER
#define PARSEC_DFA_STATE_GEN_HEADER

#include "../Grammar.hpp"

#include "State.hpp"
#include <vector>

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
		std::vector<State> run(const Grammar& grammar);
		/** @} */
	};
}

#endif