#ifndef PARSEC_LR_REDUCE_ACTION_HEADER
#define PARSEC_LR_REDUCE_ACTION_HEADER

#include "../fg/Symbol.hpp"
#include <vector>

namespace parsec::lr {
	class ReduceAction {
	public:
		/** @{ */
		ReduceAction(const fg::Symbol* symbol, int states) noexcept
			: m_symbol(symbol), m_states(states)
		{ }
		/** @} */


		/** @{ */
		ReduceAction(const ReduceAction&) = default;
		ReduceAction& operator=(const ReduceAction&) = default;
		/** @} */


		/** @{ */
		const fg::Symbol* symbol() const noexcept {
			return m_symbol;
		}

		int states() const noexcept {
			return m_states;
		}
		/** @} */


	private:
		/** @{ */
		const fg::Symbol* m_symbol;
		int m_states;
		/** @} */
	};


	using ReduceActionList = std::vector<ReduceAction>;
}

#endif