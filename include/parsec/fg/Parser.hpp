#ifndef PARSEC_FG_PARSER_HEADER
#define PARSEC_FG_PARSER_HEADER

#include "Grammar.hpp"

#include <string_view>
#include <istream>

namespace parsec::fg {
	class Parser {
	public:
		/** @{ */
		Parser() = default;
		/** @} */


		/** @{ */
		Parser(Parser&&) = default;
		Parser& operator=(Parser&&) = default;
		/** @} */

		/** @{ */
		Parser(const Parser&) = delete;
		Parser& operator=(const Parser&) = delete;
		/** @} */


		/** @{ */
		Grammar parse(std::string_view input);
		Grammar parse(std::istream& input);
		/** @} */
	};
}

#endif