#ifndef PARSEC_SRC_GEN_CPP_PARSER_GEN_HEADER
#define PARSEC_SRC_GEN_CPP_PARSER_GEN_HEADER

#include "../core/NonCopyable.hpp"
#include "ParserSpec.hpp"

#include <ostream>

namespace parsec::src_gen {

	/**
	 * @brief Generates C++ source code for a syntax analyzer based on a ParserSpec.
	*/
	class CppParserGen : private NonCopyable {
	public:

		/**
		 * @brief Create a new generator and configure it to output generated source code to the stream.
		*/
		explicit CppParserGen(std::ostream& out)
			: m_out(&out) {
		}



		/**
		 * @brief Run the generator.
		*/
		void run(const ParserSpec& parserSpec);
		


	private:
		std::ostream* m_out = {};
	};

}

#endif