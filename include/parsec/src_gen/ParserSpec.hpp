#ifndef PARSEC_SRC_GEN_PARSER_SPEC_HEADER
#define PARSEC_SRC_GEN_PARSER_SPEC_HEADER

#include "../core/NonCopyable.hpp"
#include "../fg/SymbolGrammar.hpp"

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
		void defineParseRule(std::string_view name, fg::Rule rule);
		
		

		/**
		 * @brief Check whether the specified name defines a parse rule.
		*/
		bool isParseRule(std::string_view name) const;



		/**
		 * @brief Lexicographically sorted list of all defined parse rules.
		*/
		std::span<const std::string_view> parseRules() const;


		
		/**
		 * @brief Grammar description of the parser input language.
		*/
		const fg::SymbolGrammar& inputSyntax() const {
			return m_inputSyntax;
		}



	private:
		fg::SymbolGrammar m_inputSyntax;
		
		mutable std::vector<std::string_view> m_parseRules;
		mutable bool m_parseRulesSorted = true;
	};

}

#endif