#ifndef PARSEC_RULE_HEADER
#define PARSEC_RULE_HEADER

#include <gsl/narrow>

#include <vector>
#include <algorithm>
#include <ranges>

namespace parsec {
	class Symbol;

	/**
	 * @brief Specifies an inference rule for a @ref Symbol "grammar symbol".
	 */
	class Rule {
	public:
		Rule(const Symbol* head, int id) noexcept
			: m_head(head), m_id(id)
		{ }


		/** @{ */
		Rule(const Rule&) = delete;
		Rule& operator=(const Rule&) = delete;
		/** @} */


		/** @{ */
		/** @brief Replaces all occurrences of a symbol with another symbol. */
		void replaceSymbol(const Symbol* symbol, const Symbol* replace) {
			std::ranges::replace(m_symbols, symbol, replace);
		}
		
		Rule& addSymbol(const Symbol* symbol) {
			m_symbols.push_back(symbol);
			return *this;
		}


		std::ranges::view auto symbols() const {
			return std::ranges::ref_view(m_symbols);
		}
		
		const Symbol* symbolAt(int i) const noexcept {
			return m_symbols[i];
		}
		/** @} */

		
		/** @{ */
		const Symbol* head() const noexcept {
			return m_head;
		}

		int length() const noexcept {
			return gsl::narrow_cast<int>(m_symbols.size());
		}

		int id() const noexcept {
			return m_id;
		}
		/** @} */


	private:
		std::vector<const Symbol*> m_symbols;

		const Symbol* m_head;
		int m_id;
	};
}

#endif