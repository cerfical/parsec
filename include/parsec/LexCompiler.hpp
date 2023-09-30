#ifndef PARSEC_LEX_COMPILER_HEADER
#define PARSEC_LEX_COMPILER_HEADER

#include "LexGrammar.hpp"
#include <iostream>

namespace parsec {
	/**
	 * @brief Generates source code for a lexical analyzer from a LexGrammar.
	 */
	class LexCompiler {
	public:
		/** @{ */
		/** @brief Initialize a new LexCompiler with an input LexGrammar and a @c std::ostream for output. */
		LexCompiler(const LexGrammar& grammar, std::ostream& out = std::cout) noexcept
		 : grammar(&grammar), out(&out)
		{ }

		/** @copybrief */
		~LexCompiler() = default;
		/** @} */
		
		/** @{ */
		LexCompiler(LexCompiler&&) = default;
		LexCompiler& operator=(LexCompiler&&) = default;
		/** @} */

		/** @{ */
		LexCompiler(const LexCompiler&) = delete;
		LexCompiler& operator=(const LexCompiler&) = delete;
		/** @} */

		/** @{ */
		/** @brief Start the compilation process. */
		void Compile(std::size_t indent = 0);
		/** @} */

	private:
		const LexGrammar* grammar;
		std::ostream* out;
	};
}

#endif