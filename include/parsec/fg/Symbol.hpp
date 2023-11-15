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
		friend std::ostream& operator<<(std::ostream& out, const Symbol& sym);
		/** @} */


		/** @{ */
		explicit Symbol(
			const std::string& name = "",
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


		void setId(int id) noexcept {
			m_id = id;
		}

		int id() const noexcept {
			return m_id;
		}


		void setRule(RulePtr rule) noexcept {
			m_rule = std::move(rule);
		}

		void clearRule() noexcept {
			setRule(nullptr);
		}

		RulePtr takeRule() noexcept {
			return std::move(m_rule);
		}

		Rule* rule() noexcept {
			return m_rule.get();
		}


		const Rule* rule() const noexcept {
			return m_rule.get();
		}
		
		bool hasRule() const noexcept {
			return static_cast<bool>(m_rule);
		}

		
		void setType(SymbolTypes type) noexcept {
			m_type = type;
		}
		
		SymbolTypes type() const noexcept {
			return m_type;
		}
		/** @} */


		/** @{ */
		bool definesToken() const noexcept {
			return m_type == SymbolTypes::Token;
		}

		bool definesRule() const noexcept {
			return m_type == SymbolTypes::Rule;
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