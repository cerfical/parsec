module;

#include <istream>
#include <ostream>

export module parsec:CodeGen;

import parsec.bnf;

namespace parsec {

    /**
     * @brief Generates source code for parsers based on a template.
     */
    export class CodeGen {
    public:

        CodeGen() = default;

        CodeGen(const CodeGen& other) = delete;
        CodeGen& operator=(const CodeGen& other) = delete;

        CodeGen(CodeGen&& other) noexcept = default;
        CodeGen& operator=(CodeGen&& other) noexcept = default;

        ~CodeGen() = default;


        /** @{ */
        /**
         * @brief Set an output stream to receive the generated code.
         */
        void setOutputSink(std::ostream* output) {
            output_ = output;
        }


        /**
         * @brief Set an input stream to use as the source of a template for the generated code.
         */
        void setOutputTemplateSource(std::istream* tmpl) {
            tmpl_ = tmpl;
        }


        /**
         * @brief Set an input token language for a parser.
         */
        void setTokenGrammar(const bnf::SymbolGrammar* tokens) {
            tokens_ = tokens;
        }


        /**
         * @brief Set an input syntax language for a parser.
         */
        void setRuleGrammar(const bnf::SymbolGrammar* rules) {
            rules_ = rules;
        }


        /**
         * @brief Start the generation process.
         */
        void generate();
        /** @} */


    private:
        const bnf::SymbolGrammar* tokens_ = {};
        const bnf::SymbolGrammar* rules_ = {};

        std::ostream* output_ = {};
        std::istream* tmpl_ = {};
    };

}
