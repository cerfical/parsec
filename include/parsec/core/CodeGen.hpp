#pragma once

#include "SymbolGrammar.hpp"

#include <istream>
#include <ostream>

namespace parsec {

    class CodeGen {
    public:

        CodeGen() noexcept = default;

        CodeGen(const CodeGen& other) = delete;
        CodeGen& operator=(const CodeGen& other) = delete;

        CodeGen(CodeGen&& other) noexcept = default;
        CodeGen& operator=(CodeGen&& other) noexcept = default;

        ~CodeGen() = default;


        void setOutputSink(std::ostream* output) noexcept {
            output_ = output;
        }

        void setOutputTemplateSource(std::istream* tmpl) noexcept {
            tmpl_ = tmpl;
        }

        void setTokenGrammar(const SymbolGrammar* tokens) noexcept {
            tokens_ = tokens;
        }

        void setRuleGrammar(const SymbolGrammar* rules) noexcept {
            rules_ = rules;
        }


        void generate();


    private:
        const SymbolGrammar* tokens_ = {};
        const SymbolGrammar* rules_ = {};

        std::ostream* output_ = {};
        std::istream* tmpl_ = {};
    };

}
