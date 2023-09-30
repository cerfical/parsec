#ifndef PARSEC_DFA_BUILDER_HEADER
#define PARSEC_DFA_BUILDER_HEADER

#include "RegExNodes.hpp"
#include "DfaStateMachine.hpp"

namespace parsec {
	/**
	 * @brief Builds @ref DfaStateMachine "DfaStateMachines" from regular expressions.
	 */
	class DfaBuilder {
	public:
		/** @{ */
		/** @brief Construct a new DfaBuilder that will use the specified regex as input. */
		DfaBuilder(const RegExNode& regex) noexcept
		 : regex(&regex)
		{ }

		/** @copybrief */
		~DfaBuilder() = default;
		/** @} */

		/** @{ */
		DfaBuilder(DfaBuilder&&) = default;
		DfaBuilder& operator=(DfaBuilder&&) = default;
		/** @} */

		/** @{ */
		DfaBuilder(const DfaBuilder&) = delete;
		DfaBuilder& operator=(const DfaBuilder&) = delete;
		/** @} */

		/** @{ */
		/** @brief Build a new DfaStateMachine object. */
		DfaStateMachine Build();
		/** @} */

	private:
		const RegExNode* regex;
	};
}

#endif