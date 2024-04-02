#ifndef PARSEC_REGEX_PARSER_HEADER
#define PARSEC_REGEX_PARSER_HEADER

#include "../core/NonCopyable.hpp"
#include "../core/RegularExpr.hpp"

#include <string_view>
#include <istream>

namespace parsec::regex {

	class Parser : private NonCopyable {
	public:

		RegularExpr parse(std::string_view regex);
		
		RegularExpr parse(std::istream& input);
		
	};

}

#endif