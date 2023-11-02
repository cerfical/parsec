#ifndef PARSEC_FG_SYMBOL_HEADER
#define PARSEC_FG_SYMBOL_HEADER

#include "Rule.hpp"
#include "RuleConcat.hpp"
#include "Atom.hpp"

#include <string>
#include <vector>
#include <ostream>

namespace parsec::fg {
	class Symbol {
	public:
		/** @{ */
		friend std::ostream& operator<<(std::ostream& out, const Symbol& s) {
			out << s.name();
			return out;
		}
		/** @} */


		/** @{ */
		explicit Symbol(
			const std::string& name = "",
			RulePtr rule = nullptr,
			bool terminal = true,
			int id = 0
		) : m_name(name), m_terminal(terminal), m_id(id) {
			m_rule = makeRule<RuleConcat>(
				std::move(rule),
				makeRule<Atom>('#')
			);
		}
		/** @} */


		/** @{ */
		Symbol(Symbol&&) = default;
		Symbol& operator=(Symbol&&) = default;
		/** @} */

		/** @{ */
		Symbol(const Symbol&) = delete;
		Symbol& operator=(const Symbol&) = delete;
		/** @} */


		/** @{ */
		const std::string& name() const noexcept {
			return m_name;
		}

		int id() const noexcept {
			return m_id;
		}

		bool terminal() const noexcept {
			return m_terminal;
		}
		/** @} */


		/** @{ */
		const Rule* ruleBody() const noexcept {
			return static_cast<const Atom*>(
				static_cast<const RuleConcat*>(
					m_rule.get()
				)->left()
			);
		}

		const Atom* ruleEnd() const noexcept {
			return static_cast<const Atom*>(
				static_cast<const RuleConcat*>(
					m_rule.get()
				)->right()
			);
		}

		const Rule* rule() const noexcept {
			return m_rule.get();
		}
		/** @} */


	private:
		RulePtr m_rule;
		std::string m_name;
		bool m_terminal;
		int m_id;
	};


	using SymbolList = std::vector<Symbol>;
}

#endif