#pragma once

#include "CodeGen.hpp"

namespace parsec::src_gen {

    /**
     * @brief Generates C++ source code for a parser.
     */
    class CppCodeGen : public CodeGen {
    public:

        using CodeGen::CodeGen;

    private:
        void onLexerGen(const SymbolGrammar& tokens, const ConfigStore& configs) override;

        void onParserGen(const SymbolGrammar& syntax, const ConfigStore& configs) override;

        void onPreambleGen(const ConfigStore& configs) override;
    };

}
