#pragma once

#include "../core/TextScanner.hpp"

#include "nodes/ExprNode.hpp"

#include <istream>
#include <string_view>

namespace parsec::regex {

    class Parser {
    public:

        static NodePtr parseFrom(std::string_view str);

        static NodePtr parseFrom(std::istream& in);

        static bool isMetaChar(char ch);


        Parser(const Parser& other) = delete;
        Parser& operator=(const Parser& other) = delete;

        Parser(Parser&& other) noexcept = default;
        Parser& operator=(Parser&& other) noexcept = default;

        ~Parser() = default;


        Parser() noexcept = default;

        explicit Parser(std::istream* input) noexcept
            : input_(input) {}


        NodePtr parse();


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
