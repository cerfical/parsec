#ifndef PARSEC_SRC_GEN_CPP_LEXER_GEN_HEADER
#define PARSEC_SRC_GEN_CPP_LEXER_GEN_HEADER

#include "../core/NonCopyable.hpp"
#include "LexerSpec.hpp"

#include <ostream>

namespace parsec::src_gen {

	/**
	 * @brief Generates C++ source code for a lexical analyzer based on a LexerSpec.
	*/
	class CppLexerGen : private NonCopyable {
	public:

		/**
		 * @brief Create a new generator and configure it to output generated source code to the stream.
		*/
		explicit CppLexerGen(std::ostream& out)
			: m_out(&out) {
		}



		/**
		 * @brief Run the generator.
		*/
		void run(const LexerSpec& lexerSpec);



	private:
		std::ostream* m_out = {};
	};

}

#endif