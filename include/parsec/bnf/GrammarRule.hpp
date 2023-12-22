#ifndef PARSEC_BNF_GRAMMAR_RULE_HEADER
#define PARSEC_BNF_GRAMMAR_RULE_HEADER

#include <gsl/util>

#include <string_view>
#include <vector>

namespace parsec::bnf {

	class GrammarSymbol;

	/**
	 * @brief Expresses a GrammarSymbol in terms of some combination of other GrammarSymbol%s.
	*/
	class GrammarRule {
	public:
		
		GrammarRule(const GrammarSymbol* head, int id) noexcept
			: m_head(head), m_id(id)
		{ }

		GrammarRule(const GrammarRule&) = delete;
		GrammarRule& operator=(const GrammarRule&) = delete;



		/** @{ */		
		/**
		 * @brief Add a symbol to the end of the rule body.
		*/
		void addSymbol(const GrammarSymbol* symbol) {
			m_body.push_back(symbol);
		}



		/**
		 * @brief Get the number of nonterminal symbols in the rule body.
		*/
		std::size_t nonTerminalCount() const noexcept;


		
		/**
		 * @brief Get the number of terminal symbols in the rule body. 
		*/
		std::size_t terminalCount() const noexcept;



		/**
		 * @brief Get the symbol from the rule body at the specified index, if there is one.
		*/
		const GrammarSymbol* symbolAt(gsl::index i) const noexcept {
			if(i < size()) {
				return m_body[i];
			}
			return nullptr;
		}
		
		

		/**
		 * @brief Get a symbol for the rule head.
		*/
		const GrammarSymbol* head() const noexcept {
			return m_head;
		}



		/**
		 * @brief Get the name of the head symbol.
		*/
		std::string_view name() const noexcept;



		/**
		 * @brief Get the total number of symbols in the rule body.
		*/
		std::size_t size() const noexcept {
			return m_body.size();
		}



		/**
		 * @brief Get the unique integer identifier for the rule.
		*/
		int id() const noexcept {
			return m_id;
		}
		/** @} */



	private:
		const GrammarSymbol* m_head = {};
		std::vector<const GrammarSymbol*> m_body;
		int m_id = {};
	};

}

#endif