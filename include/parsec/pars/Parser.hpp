#pragma once

#include "../utils/util_types.hpp"
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
		NodePtr parse(std::istream& input);


		/**
		 * @brief Parse text represented by an arbitrary string.
		*/
		NodePtr parse(std::string_view input);

	};

}
