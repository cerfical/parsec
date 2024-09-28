#pragma once

#include "nodes/ExprNode.hpp"

#include <istream>
#include <string_view>

namespace parsec::regex {

    class Parser {
    public:

        Parser() = delete;


        static bool isMetaChar(char ch);

        static NodePtr parse(std::string_view input);

        static NodePtr parse(std::istream& input);
    };

}
