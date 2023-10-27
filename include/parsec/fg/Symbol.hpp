#ifndef PARSEC_FG_SYMBOL_HEADER
#define PARSEC_FG_SYMBOL_HEADER

#include "Rule.hpp"

#include <string>
#include <vector>

namespace parsec::fg {
	class Symbol {
	public:
		/** @{ */
		Symbol(
			const std::string& name = "",
			RulePtr rule = nullptr,
			bool terminal = true,
			int id = 0
		)
			: m_rule(std::move(rule))
			, m_name(name)
			, m_id(id)
			, m_terminal(terminal)
		{ }
		/** @} */


		/** @{ */
		Symbol(const Symbol&) = default;
		Symbol& operator=(const Symbol&) = default;
		/** @} */

		/** @{ */
		Symbol(Symbol&&) = default;
		Symbol& operator=(Symbol&&) = default;
		/** @} */


		/** @{ */
		const std::string& name() const noexcept {
			return m_name;
		}

		const Rule* rule() const noexcept {
			return m_rule.get();
		}

		int id() const noexcept {
			return m_id;
		}

		bool terminal() const noexcept {
			return m_terminal;
		}
		/** @} */


	private:
		/** @{ */
		RulePtr m_rule;
		std::string m_name;
		int m_id;
		bool m_terminal;
		/** @} */
	};

	using SymbolList = std::vector<Symbol>;
}

#endif