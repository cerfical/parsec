#ifndef PARSEC_LR_STATE_REDUCE_HEADER
#define PARSEC_LR_STATE_REDUCE_HEADER

#include "../core/Rule.hpp"

namespace parsec::lr {
	/**
	 * @brief Defines a states reduction by a rule.
	 */
	class StateReduce {
	public:
		StateReduce(
			const Rule* reduceRule,
			int reducedStates,
			int consumedTokens
		) noexcept
			: m_reduceRule(reduceRule)
			, m_reducedStates(reducedStates)
			, m_consumedTokens(consumedTokens)
		{ }


		/** @{ */
		StateReduce(const StateReduce&) = default;
		StateReduce& operator=(const StateReduce&) = default;
		/** @} */


		/** @{ */
		const Rule* reduceRule() const noexcept {
			return m_reduceRule;
		}

		int consumedTokens() const noexcept {
			return m_consumedTokens;
		}

		int reducedStates() const noexcept {
			return m_reducedStates;
		}
		/** @} */


	private:
		const Rule* m_reduceRule;
		int m_reducedStates;
		int m_consumedTokens;
	};
}

#endif