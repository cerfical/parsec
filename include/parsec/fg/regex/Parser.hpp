#ifndef PARSEC_FG_REGEX_PARSER_HEADER
#define PARSEC_FG_REGEX_PARSER_HEADER

#include "../../core/NonCopyable.hpp"
#include "../RegularExpr.hpp"

#include <string_view>
#include <istream>

namespace parsec::fg::regex {

	class Parser : private NonCopyable {
	public:

		RegularExpr parse(std::string_view regex);
		
		RegularExpr parse(std::istream& input);
		
	};

}

#endif