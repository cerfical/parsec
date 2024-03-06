#ifndef PARSEC_SRC_GEN_PARSER_SPEC_HEADER
#define PARSEC_SRC_GEN_PARSER_SPEC_HEADER

#include "../core/NonCopyable.hpp"
#include "../fg/Grammar.hpp"

#include <unordered_set>
#include <span>
#include <vector>

namespace parsec::src_gen {
	
	/**
	 * @brief Specification of a syntax analyzer to generate.
	*/
	class ParserSpec : private NonCopyable {
	public:

		/**
		 * @brief Specify a new parse rule for the parser.
		*/
		void defineRule(const std::string& name, fg::RuleExpr rule);
		
		

		/**
		 * @brief List of all parse rules defined.
		*/
		std::span<const std::string_view> definedRules() const {
			return m_ruleNames;
		}


		
		/**
		 * @brief Check if a rule with the specified name is already defined.
		*/
		bool isRuleDefined(const std::string& name) const;


		
		/**
		 * @brief Grammar description of a language syntax recognized by the parser.
		 */
		const fg::Grammar& syntaxGrammar() const {
			return m_syntaxGrammar;
		}



	private:
		std::unordered_set<std::string> m_definedRules;
		std::vector<std::string_view> m_ruleNames;
		fg::Grammar m_syntaxGrammar;
	};

}

#endif