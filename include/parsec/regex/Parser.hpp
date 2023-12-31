#ifndef PARSEC_REGEX_PARSER_HEADER
#define PARSEC_REGEX_PARSER_HEADER

#include "ExprNode.hpp"

#include <string_view>
#include <istream>

/** @brief Contains regular expression functionality. */
namespace parsec::regex {
	/**
	 * @brief Constructs regular expressions from their textual form.
	 */
	class Parser {
	public:
		/** @brief Classifies characters as special metacharacters used in the regex syntax. */
		static bool isMetachar(char ch) noexcept;


		Parser() = default;


		Parser(Parser&&) = default;
		Parser& operator=(Parser&&) = default;

		/** @{ */
		Parser(const Parser&) = delete;
		Parser& operator=(const Parser&) = delete;
		/** @} */


		/** @{ */
		ExprPtr parse(std::string_view regex);
		ExprPtr parse(std::istream& input);
		/** @} */
	};
}

#endif