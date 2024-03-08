#ifndef PARSEC_SRC_GEN_CPP_LEXER_GEN_HEADER
#define PARSEC_SRC_GEN_CPP_LEXER_GEN_HEADER

#include "CodeGen.hpp"
#include <ostream>

namespace parsec::src_gen {

	/**
	 * @brief Generates C++ source code for a lexical analyzer.
	*/
	class CppLexerGen : public CodeGen {
	public:

		/**
		 * @brief Create a new generator and configure it to output generated source code to the stream.
		*/
		explicit CppLexerGen(std::ostream& out)
			: m_out(&out) {
		}


		void run(const fg::SymbolGrammar& inputSyntax, const ConfigStore& configs) override;


	private:
		std::ostream* m_out = {};
	};

}

#endif