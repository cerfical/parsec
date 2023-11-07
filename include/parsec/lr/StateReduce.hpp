#ifndef PARSEC_LR_STATE_REDUCE_HEADER
#define PARSEC_LR_STATE_REDUCE_HEADER

#include "../fg/Symbol.hpp"
#include <vector>

namespace parsec::lr {
	class StateReduce {
	public:
		/** @{ */
		StateReduce(const fg::Symbol* symbol, int states, int tokens) noexcept
			: m_symbol(symbol), m_states(states), m_tokens(tokens)
		{ }
		/** @} */


		/** @{ */
		StateReduce(const StateReduce&) = default;
		StateReduce& operator=(const StateReduce&) = default;
		/** @} */


		/** @{ */
		const fg::Symbol* newSymbol() const noexcept {
			return m_symbol;
		}

		int poppedTokens() const noexcept {
			return m_tokens;
		}

		int poppedStates() const noexcept {
			return m_states;
		}
		/** @} */


	private:
		const fg::Symbol* m_symbol;
		int m_states;
		int m_tokens;
	};

	using ReduceList = std::vector<StateReduce>;
}

#endif