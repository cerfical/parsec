#ifndef PARSEC_REGEX_PARSER_HEADER
#define PARSEC_REGEX_PARSER_HEADER

#include "../core/NonCopyable.hpp"
#include "nodes/ExprNode.hpp"

#include <string_view>
#include <istream>

namespace parsec::regex {

	class Parser : private NonCopyable {
	public:

		NodePtr parse(std::string_view regex);
		
		NodePtr parse(std::istream& input);
		
	};

}

#endif