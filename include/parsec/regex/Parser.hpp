#pragma once

#include "../core/TextScanner.hpp"
#include "ast/ExprNode.hpp"

#include <istream>
#include <spanstream>
#include <string_view>

namespace parsec::regex {

    /**
     * @brief Parses regular expressions into their tree representation.
     */
    class Parser {
    public:

        /**
         * @brief Parse an expression taken from a string.
         */
        static NodePtr parseFrom(std::string_view str) {
            auto in = std::ispanstream(str);
            return parseFrom(in);
        }


        /**
         * @brief Parse an expression taken from an input stream.
         */
        static NodePtr parseFrom(std::istream& in) {
            return Parser(&in).parse();
        }


        /**
         * @brief Check if a character has a special meaning in regular expression syntax.
         */
        static bool isMetaChar(char ch);


        Parser() = default;

        Parser(const Parser& other) = delete;
        Parser& operator=(const Parser& other) = delete;

        Parser(Parser&& other) noexcept = default;
        Parser& operator=(Parser&& other) noexcept = default;

        ~Parser() = default;


        /** @{ */
        /**
         * @brief Construct a new parser that has an input stream as the source of data to parse.
         */
        explicit Parser(std::istream* input)
            : input_(input) {}


        /**
         * @brief Start the parse.
         */
        NodePtr parse();
        /** @} */


    private:
        NodePtr parseExpr();
        NodePtr parseAltern();
        NodePtr parseConcat();
        NodePtr parseRepeat();
        NodePtr parseAtom();

        bool isAtom() const;

        NodePtr parseCharSet();
        NodePtr parseCharRange();

        char parseChar();
        char parseEscapeSeq();

        TextScanner input_;
    };

}
