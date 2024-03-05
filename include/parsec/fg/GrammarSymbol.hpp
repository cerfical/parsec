#ifndef PARSEC_FG_GRAMMAR_SYMBOL_HEADER
#define PARSEC_FG_GRAMMAR_SYMBOL_HEADER

#include "../core/NonCopyable.hpp"
#include "../core/typedefs.hpp"

#include "RuleExpr.hpp"
#include <string>

namespace parsec::fg {
	
	/**
	 * @brief Mirrors a syntactic construct of a language.
	*/
	class GrammarSymbol : private NonCopyable {
	public:

		GrammarSymbol() = default;

		GrammarSymbol(std::string name, RuleExpr rule, Id id)
			: m_name(std::move(name)), m_rule(std::move(rule)), m_id(id)
		{ }



		/** @{ */
		/**
		 * @brief Add an additional inference rule for the symbol.
		*/
		void addRule(RuleExpr rule) {
			m_rule.altern(std::move(rule));
		}



		/**
		 * @brief Unique string identifier for the symbol.
		*/
		const std::string& name() const {
			return m_name;
		}



		/**
		 * @brief Combined inference rule for the symbol.
		*/
		const RuleExpr& rule() const {
			return m_rule;
		}



		/**
		 * @brief Unique integer identifier for the symbol.
		*/
		Id id() const {
			return m_id;
		}
		/** @} */



	private:
		std::string m_name;
		RuleExpr m_rule;
		Id m_id = {};
	};

}

#endif