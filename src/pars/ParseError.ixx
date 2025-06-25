module;

#include "scan/SourceLoc.hpp"

#include <format>
#include <string>
#include <string_view>

export module parsec.pars:ParseError;

import parsec.text;

namespace parsec::pars {

    /**
     * @brief Describes a syntax error in the input grammar.
     */
    export class ParseError : public std::runtime_error {
    public:

        /**
         * @brief Construct a description for an *unexpected end of file*.
         *
         * @param eofLoc The location of the encountered EOF.
         */
        static ParseError unexpectedEof(const scan::SourceLoc& eofLoc) {
            return { eofLoc, "unexpected end of file" };
        }


        /**
         * @brief Construct a description for a *use of an illegal character*.
         *
         * @param chLoc The location of the character.
         * @param ch The value of the character.
         */
        static ParseError invalidChar(const scan::SourceLoc& chLoc, char ch) {
            return { chLoc, std::format("invalid character '{}'", text::escape(ch)) };
        }


        /**
         * @brief Construct a description for a *character found at an invalid position* situation.
         *
         * @param chLoc The location of the character.
         * @param ch The value of the character.
         */
        static ParseError misplacedChar(const scan::SourceLoc& chLoc, char ch) {
            std::string msg;
            switch(ch) {
                case ')':
                case '(': {
                    msg = "unmatched parenthesis";
                    break;
                }
                case '\n': {
                    msg = "unexpected end of line";
                    break;
                }
                default: {
                    msg = std::format("unexpected '{}'", text::escape(ch));
                    break;
                }
            }
            return { chLoc, msg };
        }


        /**
         * @brief Construct a description for a *token found at an invalid position* situation.
         *
         * @param tokLoc The location of the token.
         * @param tokVal The value of the token.
         */
        static ParseError misplacedToken(const scan::SourceLoc& tokLoc, std::string_view tokVal) {
            if(tokLoc) {
                return { tokLoc, std::format("unexpected \"{}\"", text::escape(tokVal)) };
            }
            return unexpectedEof(tokLoc);
        }


        /**
         * @brief Construct a description for a *mismatched token*.
         *
         * @param foundLoc The location of the token found.
         * @param expectVal The value of the expected token.
         */
        static ParseError unmatchedToken(const scan::SourceLoc& foundLoc, std::string_view expectVal) {
            return { foundLoc, std::format("{} expected, but got{}", expectVal, foundLoc ? "" : " end of file") };
        }


        /**
         * @brief Construct an error with a description and its location.
         *
         * @param loc The error location.
         * @param msg The error description.
         */
        ParseError(const scan::SourceLoc& loc, const std::string& msg)
            : std::runtime_error(msg)
            , loc_(loc) {}


        /**
         * @brief The location where the error occurred.
         */
        const scan::SourceLoc& loc() const noexcept {
            return loc_;
        }


    private:
        scan::SourceLoc loc_;
    };

}
