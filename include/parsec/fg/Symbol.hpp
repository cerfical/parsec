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

		explicit Symbol(
			const std::string& name,
			SymbolTypes type = {},
			RulePtr rule = {},
			int id = 0
		)
			: m_rule(std::move(rule))
			, m_name(name)
			, m_type(type)
			, m_id(id)
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
		void setName(const std::string& name) {
			m_name = name;
		}

		const std::string& name() const noexcept {
			return m_name;
		}

		std::string& name() noexcept {
			return m_name;
		}


		void setId(int id) noexcept {
			m_id = id;
		}

		int id() const noexcept {
			return m_id;
		}


		void setRule(RulePtr rule) noexcept {
			m_rule = std::move(rule);
		}

		const Rule* rule() const noexcept {
			return m_rule.get();
		}
		
		RulePtr& rule() noexcept {
			return m_rule;
		}

		
		void setType(SymbolTypes type) noexcept {
			m_type = type;
		}
		
		SymbolTypes type() const noexcept {
			return m_type;
		}
		/** @} */


		/** @{ */
		bool isTerminal() const noexcept {
			return m_type == SymbolTypes::Terminal;
		}

		bool isNonterminal() const noexcept {
			return m_type == SymbolTypes::Nonterminal;
		}

		bool isWs() const noexcept {
			return m_type == SymbolTypes::Ws;
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