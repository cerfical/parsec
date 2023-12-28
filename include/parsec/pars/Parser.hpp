#ifndef PARSEC_PARS_PARSER_HEADER
#define PARSEC_PARS_PARSER_HEADER

#include "ast/Node.hpp"

#include <string_view>
#include <istream>

namespace parsec::pars {

	/**
	 * @brief Performs syntax analysis.
	*/
	class Parser {
	public:
		
		Parser() = default;

		Parser(const Parser&) = delete;
		Parser& operator=(const Parser&) = delete;

		Parser(Parser&&) = default;
		Parser& operator=(Parser&&) = default;



		/** @{ */
		/**
		 * @brief Parse text taken from the input stream.
		*/
		ast::NodePtr parse(std::istream& input);



		/**
		 * @brief Parse text represented by an arbitrary string.
		*/
		ast::NodePtr parse(std::string_view input);
		/** @} */

	};

}

#endif