#ifndef PARSEC_FG_SYMBOL_HEADER
#define PARSEC_FG_SYMBOL_HEADER

#include "../utils/SourceLoc.hpp"

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
		Symbol(
			const std::string& name = "",
			RulePtr rule = nullptr,
			bool terminal = true,
			int id = 0,
			const SourceLoc& loc = {}
		) : m_name(name), m_terminal(terminal), m_loc(loc), m_id(id) {
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
		
		const SourceLoc& loc() const noexcept {
			return m_loc;
		}
		/** @} */


	private:
		/** @{ */
		RulePtr m_rule;
		std::string m_name;
		bool m_terminal;
		/** @} */

		/** @{ */
		SourceLoc m_loc;
		int m_id;
		/** @} */
	};


	using SymbolList = std::vector<Symbol>;
}

#endif