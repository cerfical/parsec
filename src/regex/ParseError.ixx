module;

#include <format>
#include <string>

export module parsec.regex:ParseError;

import parsec.scan;
import parsec.text;

namespace parsec::regex {

    /**
     * @brief Describes a syntax error in a regular expression.
     */
    export class ParseError : public std::runtime_error {
    public:

        /**
         * @brief Construct an error description for the unexpected end-of-file.
         *
         * @param loc The error location.
         */
        static ParseError unexpectedEof(const scan::SourceLoc& loc) {
            return { loc, "unexpected end of file" };
        }


        /**
         * @brief Construct an error description for an empty hexadecimal escape sequence.
         *
         * @param loc The error location.
         */
        static ParseError emptyHexEscapeSeq(const scan::SourceLoc& loc) {
            return { loc, "expected at least one hexadecimal digit" };
        }


        /**
         * @brief Construct an error description for an out-of-order character range.
         *
         * @param loc The error location.
         */
        static ParseError outOfOrderCharRange(const scan::SourceLoc& loc) {
            return { loc, "character range is out of order" };
        }


        /**
         * @brief Construct an error description for a misplaced character.
         *
         * @param loc The error location.
         * @param ch The misplaced character.
         */
        static ParseError misplacedChar(const scan::SourceLoc& loc, char ch) {
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
            return { loc, msg };
        }


        /**
         * @brief Construct a new error with a description and its location.
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
