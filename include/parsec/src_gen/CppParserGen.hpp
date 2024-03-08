#ifndef PARSEC_SRC_GEN_CPP_PARSER_GEN_HEADER
#define PARSEC_SRC_GEN_CPP_PARSER_GEN_HEADER

#include "CodeGen.hpp"
#include <ostream>

namespace parsec::src_gen {

	/**
	 * @brief Generates C++ source code for a syntax analyzer.
	*/
	class CppParserGen : public CodeGen {
	public:

		/**
		 * @brief Create a new generator and configure it to output generated source code to the stream.
		*/
		explicit CppParserGen(std::ostream& out)
			: m_out(&out) {
		}


		void run(const fg::SymbolGrammar& inputSyntax, const ConfigStore& configs) override;


	private:
		std::ostream* m_out = {};
	};

}

#endif