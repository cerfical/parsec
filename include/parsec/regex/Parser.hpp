#pragma once

#include "../utils/util_types.hpp"
#include "nodes/ExprNode.hpp"

#include <string_view>
#include <istream>

namespace parsec::regex {

	class Parser : private NonCopyable {
	public:

		static bool isMetaChar(char ch);


		NodePtr parse(std::string_view regex);
		
		NodePtr parse(std::istream& input);
		
	};

}
