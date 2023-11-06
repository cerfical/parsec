#ifndef PARSEC_FG_SYMBOL_HEADER
#define PARSEC_FG_SYMBOL_HEADER

#include "SymbolTypes.hpp"
#include "Rule.hpp"

#include <string>
#include <vector>
#include <ostream>

namespace parsec::fg {
	class Symbol {
	public:
		/** @{ */
		friend std::ostream& operator<<(std::ostream& out, const Symbol& sym) {
			out << sym.name();
			return out;
		}
		/** @} */


		/** @{ */
		Symbol() = default;

		Symbol(const std::string& name, RulePtr rule, SymbolTypes type, int id)
			: m_rule(std::move(rule)), m_name(name), m_type(type), m_id(id)
		{ }
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

		const Rule* rule() const noexcept {
			return m_rule.get();
		}
		/** @} */


		/** @{ */
		SymbolTypes type() const noexcept {
			return m_type;
		}

		bool isTerminal() const noexcept {
			return m_type == SymbolTypes::Terminal;
		}

		bool isNonterminal() const noexcept {
			return m_type == SymbolTypes::Nonterminal;
		}

		bool isStart() const noexcept {
			return m_type == SymbolTypes::Start;
		}

		bool isEnd() const noexcept {
			return m_type == SymbolTypes::End;
		}
		/** @} */


	private:
		RulePtr m_rule;
		std::string m_name;
		SymbolTypes m_type = SymbolTypes::End;
		int m_id = 0;
	};


	using SymbolList = std::vector<const Symbol*>;
}

#endif