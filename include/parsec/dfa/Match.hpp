#ifndef PARSEC_DFA_MATCH_HEADER
#define PARSEC_DFA_MATCH_HEADER

#include "../fg/Symbol.hpp"
#include <vector>

namespace parsec::dfa {
	class Match {
	public:
		/** @{ */
		explicit Match(const fg::Symbol* symbol) noexcept
			: m_symbol(symbol)
		{ }
		/** @} */


		/** @{ */
		Match(const Match&) = default;
		Match& operator=(const Match&) = default;
		/** @} */


		/** @{ */
		const fg::Symbol* symbol() const noexcept {
			return m_symbol;
		}
		/** @} */


	private:
		/** @{ */
		const fg::Symbol* m_symbol;
		/** @} */
	};


	using MatchList = std::vector<Match>;
}

#endif