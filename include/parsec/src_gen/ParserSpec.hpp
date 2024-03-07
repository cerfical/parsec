#ifndef PARSEC_SRC_GEN_PARSER_SPEC_HEADER
#define PARSEC_SRC_GEN_PARSER_SPEC_HEADER

#include "../core/NonCopyable.hpp"
#include "../fg/SymbolGrammar.hpp"

namespace parsec::src_gen {
	
	/**
	 * @brief Specification of a syntax analyzer to generate.
	*/
	class ParserSpec : private NonCopyable {
	public:

		/**
		 * @brief Specify a new parse rule for the parser.
		*/
		void defineParseRule(std::string_view name, fg::Rule rule) {
			m_parseRules.insertSymbol(name, std::move(rule));
		}
		
		

		/**
		 * @brief Check whether the specified name defines a parse rule.
		*/
		bool isParseRule(std::string_view name) const {
			if(m_parseRules.resolveSymbol(name)) {
				return true;
			}
			return false;
		}


		
		/**
		 * @brief Complete set of all parse rules for the parser.
		*/
		const fg::SymbolGrammar& parseRules() const {
			return m_parseRules;
		}



	private:
		fg::SymbolGrammar m_parseRules;
	};

}

#endif