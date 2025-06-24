#pragma once

#include "../core/SourceLoc.hpp"

#include <string>

namespace parsec::regex {

    /**
     * @brief Describes a syntax error in a regular expression.
     */
    class ParseError : public std::runtime_error {
    public:

        /**
         * @brief Construct an error description for the unexpected end-of-file.
         *
         * @param loc The error location.
         */
        static ParseError unexpectedEof(const SourceLoc& loc);


        /**
         * @brief Construct an error description for an empty hexadecimal escape sequence.
         *
         * @param loc The error location.
         */
        static ParseError emptyHexEscapeSeq(const SourceLoc& loc);


        /**
         * @brief Construct an error description for an out-of-order character range.
         *
         * @param loc The error location.
         */
        static ParseError outOfOrderCharRange(const SourceLoc& loc);


        /**
         * @brief Construct an error description for a misplaced character.
         *
         * @param loc The error location.
         * @param ch The misplaced character.
         */
        static ParseError misplacedChar(const SourceLoc& loc, char ch);


        /**
         * @brief Construct a new error with a description and its location.
         *
         * @param loc The error location.
         * @param msg The error description.
         */
        ParseError(const SourceLoc& loc, const std::string& msg)
            : std::runtime_error(msg)
            , loc_(loc) {}


        /**
         * @brief The location where the error occurred.
         */
        const SourceLoc& loc() const noexcept {
            return loc_;
        }


    private:
        SourceLoc loc_;
    };

}
