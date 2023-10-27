#ifndef PARSEC_REGEX_PARSER_HEADER
#define PARSEC_REGEX_PARSER_HEADER

#include "../fg/Rule.hpp"

#include <string_view>
#include <istream>

namespace parsec::regex {
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
		fg::RulePtr parse(std::string_view regex);
		fg::RulePtr parse(std::istream& input);
		/** @} */
	};
}

#endif