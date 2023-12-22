#ifndef PARSEC_BNF_GRAMMAR_SYMBOL_HEADER
#define PARSEC_BNF_GRAMMAR_SYMBOL_HEADER

#include "GrammarRule.hpp"

#include <gsl/narrow>

#include <string>
#include <string_view>
#include <deque>

namespace parsec::bnf {
	
	/**
	 * @brief Mirrors a syntactic construct of a language.
	*/
	class GrammarSymbol {
	public:

		using RuleList = std::deque<GrammarRule>;



		GrammarSymbol(std::string_view name, int id)
			: m_name(name), m_id(id)
		{ }

		GrammarSymbol(const GrammarSymbol&) = delete;
		GrammarSymbol& operator=(const GrammarSymbol&) = delete;



		/** @{ */
		/**
		 * @brief Insert an empty GrammarRule to the symbol.
		*/
		GrammarRule* addRule() {
			m_rules.emplace_back(this, uniqueRuleId());
			return &m_rules.back();
		}



		/**
		 * @brief Get a list of all inference rules defined for the symbol.
		*/
		const RuleList& rules() const noexcept {
			return m_rules;
		}


		
		/**
		 * @brief Check whether the symbol is non-terminal.
		*/
		bool isNonTerminal() const noexcept {
			return !isTerminal();
		}



		/**
		 * @brief Check whether the symbol is terminal, that is, doesn't have any inference rules specified.
		*/
		bool isTerminal() const noexcept {
			return m_rules.empty();
		}
		


		/**
		 * @brief Get the string identifier for the symbol.
		*/
		std::string_view name() const noexcept {
			return m_name;
		}



		/**
		 * @brief Get the unique integer identifier for the symbol.
		*/
		int id() const noexcept {
			return m_id;
		}
		/** @} */



	private:
		int uniqueRuleId() const noexcept {
			return gsl::narrow_cast<int>(m_rules.size());
		}

		RuleList m_rules;
		std::string m_name;
		int m_id = {};
	};

}

#endif