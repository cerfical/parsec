#ifndef PARSEC_SRC_GEN_CODE_GEN_HEADER
#define PARSEC_SRC_GEN_CODE_GEN_HEADER

#include "../core/NonCopyable.hpp"
#include "../fg/SymbolGrammar.hpp"

#include "ConfigStore.hpp"
#include <ostream>

namespace parsec::src_gen {

	/**
	 * @brief Provides a simple interface for generating source code for parsers.
	*/
	class CodeGen : private NonCopyable {
	public:

		CodeGen() = default;

		/**
		 * @brief Create a new generator and configure it to output generated code to a stream.
		*/
		explicit CodeGen(std::ostream& out)
			: m_out(&out) {}


		virtual ~CodeGen() = default;


		/**
		 * @brief Run the generator to produce code.
		 * @param tokens Specification of the language tokens that the parser will operate on.
		 * @param syntax Grammar description of the language recognized by the parser.
		 * @param configs Additional configuration parameters.
		*/
		void run(const fg::SymbolGrammar& tokens, const fg::SymbolGrammar& syntax, const ConfigStore& configs = {});


	protected:

		/**
		 * @brief Called to generate the lexical analyzer part of the parser.
		*/
		virtual std::string onLexerGen(const fg::SymbolGrammar& tokens, const ConfigStore& configs) = 0;
		

		/**
		 * @brief Called to generate the syntax analyzer part of the parser.
		*/
		virtual std::string onParserGen(const fg::SymbolGrammar& syntax, const ConfigStore& configs) = 0;


		/**
		 * @brief Called to produce additional dependencies for the generated code.
		*/
		virtual std::string onPreambleGen(const ConfigStore& configs) = 0;


	private:
		std::ostream* m_out = {};
	};

}

#endif