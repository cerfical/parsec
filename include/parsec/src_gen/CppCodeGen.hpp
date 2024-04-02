#ifndef PARSEC_SRC_GEN_CPP_CODE_GEN_HEADER
#define PARSEC_SRC_GEN_CPP_CODE_GEN_HEADER

#include "CodeGen.hpp"

namespace parsec::src_gen {

	/**
	 * @brief Generates C++ source code for a parser.
	*/
	class CppCodeGen : public CodeGen {
	public:

		using CodeGen::CodeGen;

	private:
		void onLexerGen(const fg::SymbolGrammar& tokens, const ConfigStore& configs) override;

		void onParserGen(const fg::SymbolGrammar& syntax, const ConfigStore& configs) override;

		void onPreambleGen(const ConfigStore& configs) override;
	};

}

#endif