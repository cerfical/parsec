#ifndef PARSEC_PARS_PARSER_HEADER
#define PARSEC_PARS_PARSER_HEADER

#include "../core/NonCopyable.hpp"
#include "ast/Node.hpp"

#include <string_view>
#include <istream>

namespace parsec::pars {

	/**
	 * @brief Performs syntax analysis.
	*/
	class Parser : private NonCopyable {
	public:
		
		/**
		 * @brief Parse text taken from a stream.
		*/
		ast::NodePtr parse(std::istream& input);


		/**
		 * @brief Parse text represented by an arbitrary string.
		*/
		ast::NodePtr parse(std::string_view input);

	};

}

#endif