#pragma once

#include <istream>
#include <ostream>
#include <string_view>

namespace parsec {

    /**
     * @brief Produces source code for lexers/parsers based on textual specifications.
     */
    class Compiler {
    public:

        Compiler(const Compiler&) = delete;
        Compiler& operator=(const Compiler&) = delete;

        Compiler(Compiler&&) = default;
        Compiler& operator=(Compiler&&) = default;

        ~Compiler() = default;


        /** @{ */
        /**
         * @brief Create a compiler and direct its output to a stream.
         *
         * @param output The target output stream.
         */
        explicit Compiler(std::ostream& output)
            : output_(&output) {}


        /**
         * @brief Perform the compilation on data read from a `std::istream`.
         *
         * @param input A stream with the desired input grammar.
         */
        void compile(std::istream& input);


        /**
         * @brief Perform the compilation on a string.
         *
         * @param str An input grammar in the form of a string.
         */
        void compile(std::string_view str);
        /** @} */


    private:
        std::ostream* output_ = {};
    };

}
