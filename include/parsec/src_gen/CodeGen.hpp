#ifndef PARSEC_SRC_GEN_CODE_GEN_HEADER
#define PARSEC_SRC_GEN_CODE_GEN_HEADER

#include "../core/NonCopyable.hpp"
#include "../core/NonMovable.hpp"
#include "../fg/SymbolGrammar.hpp"

#include "ConfigStore.hpp"

namespace parsec::src_gen {

	/**
	 * @brief Provides a simple interface for generating source code for language recognizers.
	*/
	class CodeGen : private NonCopyable, private NonMovable {
	public:

		virtual ~CodeGen() = default;


		/**
		 * @brief Run the generator to produce source code for a recognizer.
		 * @param inputSyntax Specification of the input language the recognizer will accept.
		 * @param configs Additional configuration parameters.
		*/
		virtual void run(const fg::SymbolGrammar& inputSyntax, const ConfigStore& configs = {}) = 0;

	};

}

#endif