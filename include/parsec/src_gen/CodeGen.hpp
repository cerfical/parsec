#ifndef PARSEC_SRC_GEN_CODE_GEN_HEADER
#define PARSEC_SRC_GEN_CODE_GEN_HEADER

#include "../core/NonCopyable.hpp"
#include "../fg/SymbolGrammar.hpp"

#include "ConfigStore.hpp"
#include <iostream>

namespace parsec::src_gen {

	/**
	 * @brief Provides a simple interface for generating source code for parsers.
	*/
	class CodeGen : private NonCopyable {
	public:

		/**
		 * @brief Create a new generator with the specified output stream installed.
		*/
		explicit CodeGen(std::ostream& output = std::cout)
			: m_out(&output) {}


		virtual ~CodeGen() = default;


		/**
		 * @brief Run the generator to produce code.
		 * @param tokens Specification of the language tokens that the parser will operate on.
		 * @param syntax Grammar description of the language recognized by the parser.
		 * @param configs Additional configuration parameters.
		*/
		void run(const fg::SymbolGrammar& tokens, const fg::SymbolGrammar& syntax, const ConfigStore& configs = {}) {
			onPreambleGen(configs);
			onLexerGen(tokens, configs);
			onParserGen(syntax, configs);
		}


		/**
		 * @brief Output stream where the generated data will go.
		*/
		std::ostream& output() const {
			return *m_out;
		}


	protected:

		/**
		 * @brief Called to generate the lexical analyzer part of the parser.
		*/
		virtual void onLexerGen(const fg::SymbolGrammar& tokens, const ConfigStore& configs) = 0;
		

		/**
		 * @brief Called to generate the syntax analyzer part of the parser.
		*/
		virtual void onParserGen(const fg::SymbolGrammar& syntax, const ConfigStore& configs) = 0;


		/**
		 * @brief Called to produce additional dependencies for the generated code.
		*/
		virtual void onPreambleGen(const ConfigStore& configs) = 0;


	private:
		std::ostream* m_out = {};
	};

}

#endif