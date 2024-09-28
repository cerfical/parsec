#pragma once

#include "ast/Node.hpp"

#include <istream>
#include <string_view>

namespace parsec::pars {

    /**
     * @brief Performs syntax analysis.
     */
    class Parser {
    public:

        Parser() = delete;


        /**
         * @brief Parse text taken from a stream.
         */
        static NodePtr parse(std::istream& input);


        /**
         * @brief Parse text represented by an arbitrary string.
         */
        static NodePtr parse(std::string_view input);
    };

}
