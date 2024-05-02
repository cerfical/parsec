#ifndef PARSEC_CORE_COMPILER_HEADER
#define PARSEC_CORE_COMPILER_HEADER

#include "../utils/util_types.hpp"

#include <string_view>
#include <istream>
#include <ostream>

namespace parsec {

	/**
	 * @brief Produces source code for lexers/parsers based on textual specifications.
	*/
	class Compiler : private NonCopyable {
	public:

		/**
		 * @brief Create a compiler and direct its output to the stream.
		*/
		explicit Compiler(std::ostream& output)
			: m_output(&output) {
		}



		/** @{ */
		/**
		 * @brief Perform the compilation on the specified text source.
		*/
		void compile(std::istream& input);

		void compile(std::string_view str);
		/** @} */



	private:
		std::ostream* m_output = {};
	};

}

#endif