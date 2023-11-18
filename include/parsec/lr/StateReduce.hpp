#ifndef PARSEC_LR_STATE_REDUCE_HEADER
#define PARSEC_LR_STATE_REDUCE_HEADER

#include "../Symbol.hpp"

namespace parsec::lr {
	class StateReduce {
	public:
		/** @{ */
		StateReduce(
			const Symbol* newSymbol,
			int reducedStates,
			int consumedTokens
		) noexcept
			: m_newSymbol(newSymbol)
			, m_reducedStates(reducedStates)
			, m_consumedTokens(consumedTokens)
		{ }
		/** @} */


		/** @{ */
		StateReduce(const StateReduce&) = default;
		StateReduce& operator=(const StateReduce&) = default;
		/** @} */


		/** @{ */
		const Symbol* newSymbol() const noexcept {
			return m_newSymbol;
		}

		int consumedTokens() const noexcept {
			return m_consumedTokens;
		}

		int reducedStates() const noexcept {
			return m_reducedStates;
		}
		/** @} */


	private:
		const Symbol* m_newSymbol;
		int m_reducedStates;
		int m_consumedTokens;
	};
}

#endif