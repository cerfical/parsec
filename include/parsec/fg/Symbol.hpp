#ifndef PARSEC_FG_SYMBOL_HEADER
#define PARSEC_FG_SYMBOL_HEADER

#include "SymbolTypes.hpp"
#include "RuleConcat.hpp"
#include "Atom.hpp"

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
		Symbol(const std::string& name, RulePtr rule, SymbolTypes type, int id)
			: m_name(name), m_type(type), m_id(id) {
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


		/** @{ */
		SymbolTypes type() const noexcept {
			return m_type;
		}

		bool isTerminal() const noexcept {
			return m_type == SymbolTypes::Terminal || m_type == SymbolTypes::End;
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
		SymbolTypes m_type;
		int m_id;
	};


	using SymbolList = std::vector<const Symbol*>;
}

#endif